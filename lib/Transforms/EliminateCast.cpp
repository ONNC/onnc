//===- EliminateCast.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Config/ONNX.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Transforms/EliminateCast.h>

using namespace onnc;

template <typename SrcType>
void castTensorValues(xTensor& dest_tensor, const xTensor& src_tensor, const std::vector<SrcType>& src_data)
{
  const std::vector<SrcType>* src_data_ptr;

  std::vector<SrcType> processed_data;
  if (src_tensor.is_raw_data()) {
    const SrcType *raw_ptr = reinterpret_cast<const SrcType*>(src_tensor.raw().c_str());
    for (size_t i = 0; i < src_tensor.raw().length() / sizeof(SrcType); ++i) {
      processed_data.emplace_back(raw_ptr[i]);
    }
    src_data_ptr = &processed_data;
  }
  else {
    src_data_ptr = &src_data;
  }

  const ONNX_NAMESPACE::TensorProto_DataType& casted_type = dest_tensor.elem_type();

  switch(casted_type) {
  case ONNX_NAMESPACE::TensorProto_DataType_UNDEFINED:
    assert(false && "Output's data type is undefined.");
  case ONNX_NAMESPACE::TensorProto_DataType_FLOAT16:
  case ONNX_NAMESPACE::TensorProto_DataType_FLOAT:
    for (auto& i : *src_data_ptr)  {
      dest_tensor.floats().emplace_back(i);
    }
    break;
  case ONNX_NAMESPACE::TensorProto_DataType_DOUBLE:
    for (auto& i : *src_data_ptr)  {
      dest_tensor.doubles().emplace_back(i);
    }
    break;
  case ONNX_NAMESPACE::TensorProto_DataType_BOOL: 
  case ONNX_NAMESPACE::TensorProto_DataType_INT8: 
  case ONNX_NAMESPACE::TensorProto_DataType_INT16:
  case ONNX_NAMESPACE::TensorProto_DataType_INT32:
  case ONNX_NAMESPACE::TensorProto_DataType_UINT8:
  case ONNX_NAMESPACE::TensorProto_DataType_UINT16:
    for (auto& i : *src_data_ptr)  {
      dest_tensor.int32s().emplace_back(i);
    }
    break;
  case ONNX_NAMESPACE::TensorProto_DataType_INT64:
    for (auto& i : *src_data_ptr)  {
      dest_tensor.int64s().emplace_back(i);
    }
    break;
  case ONNX_NAMESPACE::TensorProto_DataType_UINT32:
  case ONNX_NAMESPACE::TensorProto_DataType_UINT64:
    for (auto& i : *src_data_ptr)  {
      dest_tensor.uint64s().emplace_back(i);
    }
    break;
  default:
    assert(false && "Operation does not support for this data type.");
  }
}

Pass::ReturnType EliminateCast::runOnModule(::onnc::Module &pModule)
{
  xGraph *graph = pModule.getRootTensorGraph();
  Pass::ReturnType isChanged = Pass::kModuleNoChanged;

  for (auto it = graph->begin(); it != graph->end(); ++it) {
    auto *node = *it;
    if (node->kind() == xSymbol("Cast")) {
      // Input tensor should be from Initializer, or continue
      const xValue* cast_input_value = node->input();
      if (graph->getInitializer(cast_input_value->uniqueName()) == graph->initializers().end()) {
        continue;
      }

      isChanged |= Pass::kModuleChanged;
      const xTensor& cast_input_tensor = *(graph->getInitializer(cast_input_value->uniqueName()));

      const ONNX_NAMESPACE::TensorProto_DataType original_type = cast_input_value->elemType();
      const ONNX_NAMESPACE::TensorProto_DataType casted_type = static_cast<ONNX_NAMESPACE::TensorProto_DataType>(node->i(xSymbol("to")));
      
      // Create a new tensor to store the casted tensor values from Cast input tensor, and replace Cast output tensor.
      xTensor casted_tensor;
      casted_tensor.elem_type() = casted_type;
      casted_tensor.sizes() = cast_input_tensor.sizes();

      switch(original_type) {
      case ONNX_NAMESPACE::TensorProto_DataType_UNDEFINED:
        assert(false && "Input's data type is undefined.");
      case ONNX_NAMESPACE::TensorProto_DataType_FLOAT16:
      case ONNX_NAMESPACE::TensorProto_DataType_FLOAT:
        castTensorValues(casted_tensor, cast_input_tensor, cast_input_tensor.floats());
        break;
      case ONNX_NAMESPACE::TensorProto_DataType_DOUBLE:
        castTensorValues(casted_tensor, cast_input_tensor, cast_input_tensor.doubles());
        break;
      case ONNX_NAMESPACE::TensorProto_DataType_BOOL: 
      case ONNX_NAMESPACE::TensorProto_DataType_INT8: 
      case ONNX_NAMESPACE::TensorProto_DataType_INT16:
      case ONNX_NAMESPACE::TensorProto_DataType_INT32:
      case ONNX_NAMESPACE::TensorProto_DataType_UINT8:
      case ONNX_NAMESPACE::TensorProto_DataType_UINT16:
        castTensorValues(casted_tensor, cast_input_tensor, cast_input_tensor.int32s());
        break;
      case ONNX_NAMESPACE::TensorProto_DataType_INT64:
        castTensorValues(casted_tensor, cast_input_tensor, cast_input_tensor.int64s());
        break;
      case ONNX_NAMESPACE::TensorProto_DataType_UINT32:
      case ONNX_NAMESPACE::TensorProto_DataType_UINT64:
        castTensorValues(casted_tensor, cast_input_tensor, cast_input_tensor.uint64s());
        break;
      default:
        assert(false && "Operation does not support for this data type.");
      }
      /*
        Initializer                            Initializer
             |                                      |
        [input_tensor]                        [input_tensor] 
             |            
            Cast          ->    Initializer
             |                       |       
        [output_tensor]       [casted_tensor]
             |                       |
        Next Operator           Next Operator
      */
      xValue* casted = graph->addInitializerAndInput(casted_tensor);

      node->outputs()[0]->replaceAllUsesWith(casted);
      dropOutputs(*node);
      it.destroyCurrent();
    }
  }
  return isChanged;
}

ModulePass *onnc::CreateEliminateCastPass()
{
  return new EliminateCast();
}
