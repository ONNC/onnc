//===- UpdateGraphOutputSize.cpp ------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/IR/IRBuilder.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Support/IOStream.h>
#include <onnx/checker.h>
#include <onnx/shape_inference/implementation.h>

using TensorSizes = std::vector<::onnx::Dimension>;
using TP_DataTy = ::onnx::TensorProto_DataType;

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//

static void UpdateOutputInfo(::onnx::Node *pNode, const TensorSizes &pSize,
                             TP_DataTy pTy)
{
  for (::onnx::Value* out : pNode->outputs()) {
    out->setElemType(pTy);
    if (out->sizes().empty())
      out->setSizes(pSize);
  }
}

static void UpdateReshapeOutputInfo(::onnx::Node *pNode)
{
  // second input is a shape tensor
  const ::onnx::Value *input1 = pNode->inputs()[1];
  const ::onnx::Tensor shapeTensor =
      getTensor(input1->uniqueName(), *pNode->owningGraph());
  TensorSizes dims;

  // tensor is raw data
  if (0 == shapeTensor.int64s().size()) {
    size_t size = getTotalCount(shapeTensor.sizes());
    std::vector<int64_t> int64Dims;
    int64Dims.reserve(size);
    int64Dims.resize(size);
    memcpy(int64Dims.data(), shapeTensor.raw().data(), size * sizeof(int64_t));
    for (auto &i : int64Dims) {
      dims.push_back(::onnx::Dimension(i));
    }
  } else {
    for (auto &i : shapeTensor.int64s()) {
      dims.push_back(::onnx::Dimension(i));
    }
  }
  // First input is the data tensor
  const ::onnx::Value *in = pNode->inputs()[0];
  UpdateOutputInfo(pNode, dims, in->elemType());
}

//===----------------------------------------------------------------------===//
// UpdateGraphOutputSize
//===----------------------------------------------------------------------===//
UpdateGraphOutputSize::UpdateGraphOutputSize()
  : ModulePass(ID) {
}

Pass::ReturnType UpdateGraphOutputSize::runOnModule(Module& pModule)
{
  for (::onnx::Node *n : pModule.getGraphIR()->nodes()) {
    const auto kind = n->kind();
    if (kind == ::onnx::kReshape) {
      UpdateReshapeOutputInfo(n);
    }
  }

  // use onnx official shape inference implementation
  ::onnx::ModelProto modelProto;
  ::onnc::ExportModelProto(modelProto, pModule);
  try {
    ::onnx::checker::check_model(modelProto);
  } catch (::onnx::checker::ValidationError &e) {
    std::cerr << e.what() << std::endl
              << "ONNXShapeInference pass is not workable!!" << std::endl;
    return Pass::kModuleNoChanged;
  }
  ::onnx::shape_inference::InferShapes(modelProto);
  ::onnc::IRBuilder ir_b(pModule);
  ir_b.update(modelProto);

  return Pass::kModuleChanged;
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char UpdateGraphOutputSize::ID = 0;

namespace onnc
{
  INITIALIZE_PASS(UpdateGraphOutputSize, "UpdateGraphOutputSize")
}

UpdateGraphOutputSize *onnc::CreateUpdateGraphOutputSizePass()
{
  return new UpdateGraphOutputSize();
}
