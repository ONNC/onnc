#include "TGConv.h"
#include "TGRelu.h"
#include "TGLRN.h"
#include "TGMaxPool.h"
#include "TGGemm.h"
#include "TGSoftmax.h"
#include "TGISelLowering.h"

// TODO move TargetLowering to onnc/CodeGen
void TargetLowering::CodeGenAndEmitInst(
    onnx::Graph *onnxGraph, std::vector<std::unique_ptr<Operator> > &instList) {
  instList.clear();
  for (auto it = onnxGraph->begin(), ie = onnxGraph->end(); it != ie;
       ++it) {
    const onnx::Node *const node = *it;
    LowerOperation(*node, instList);
  }
}

void TGTargetLowering::ddrScanAndAlloc(MemTable &memTable, onnx::Graph &graph) {
  // Definition fom BM168xBackendContext.hpp
  // TAG will be masked by runtime while processing cmdbuf.
  const int GLOBAL_NEURON_TAG = 0x1;
  const int GLOBAL_WEIGHT_TAG = 0x2;
  const int GLOBAL_ARM_TAG = 0x3;

  // allocate spaces for weight
  unsigned int weight_offset = 0;
  // BMKernel only supports DATA_FMT_F32 & DATA_FMT_I1
  int F32_SIZE = 4;
  std::string tab = "\t";

  std::cout << __func__ << " dump global memory layout:" << std::endl;

  for (auto i : graph.initializers()) {

    memTable[i.name()] = weight_offset + GLOBAL_WEIGHT_TAG;
    std::cout << tab << i.name() << " = " << weight_offset;

    assert(i.elem_type() == onnx::TensorProto_DataType_FLOAT);
    if (i.sizes().size() > 0) {
      int tensor_size = F32_SIZE;
      std::cout << " <";
      for (auto dim : i.sizes()) {
        std::cout << dim << ",";
        tensor_size *= dim;
      }
      std::cout << ">" << std::endl;
      weight_offset += tensor_size;
    } else {
      std::cout << std::endl;
    }
  }

  unsigned int neuron_offset = 0;
  std::unordered_set<std::string> initNames(graph.initializer_names().begin(),
                                            graph.initializer_names().end());
  // allocate space for inputs
  for (auto i : graph.inputs()) {
    if (0 == initNames.count(i->uniqueName())) {

      memTable[i->uniqueName()] = neuron_offset + GLOBAL_NEURON_TAG;
      std::cout << tab << i->uniqueName() << " = " << neuron_offset;

      assert(i->elemType() == onnx::TensorProto_DataType_FLOAT);
      if (i->sizes().size() > 0) {
        int tensor_size = F32_SIZE;
        std::cout << " <";
        for (auto &dim : i->sizes()) {
          std::cout << dim.dim << ",";
          tensor_size *= dim.dim;
        }
        std::cout << ">" << std::endl;
        neuron_offset += tensor_size;
      } else {
        std::cout << std::endl;
      }
    }
  }
  // allocate space for outputs
  for (auto i : graph.nodes()) {
    if (i->kind() == onnx::Symbol("Undefined"))
      continue;

    for (auto o : i->outputs()) {

      memTable[o->uniqueName()] = neuron_offset + GLOBAL_NEURON_TAG;
      std::cout << tab << o->uniqueName() << " = " << neuron_offset;

      // FIXME: remove this after output dimension is fixed
      assert(o->elemType() == onnx::TensorProto_DataType_FLOAT);
      if (o->sizes().size() > 0) {
        int tensor_size = F32_SIZE;
        std::cout << " <";
        for (auto dim : o->sizes()) {
          std::cout << dim.dim << ",";
          tensor_size *= dim.dim;
        }
        std::cout << ">" << std::endl;
        neuron_offset += tensor_size;
      } else {
        std::cout << std::endl;
      }
    }
  }
  std::cout << tab << "weight size: " << weight_offset << std::endl;
  std::cout << tab << "neuron size: " << neuron_offset << std::endl;
}

TGTargetLowering::TGTargetLowering(onnx::Graph *onnxGraph) {
  // plan global memory layout
  ddrScanAndAlloc(m_globalMemLayout, *onnxGraph);
}

Operator *TGTargetLowering::LowerHelper(const onnx::Node &node,
                                        MemTable &memTable) {
  uint32_t symbol = node.kind();
  if (symbol == onnx::Symbol("Conv"))
    return new TGConv(node, memTable);
  else if (symbol == onnx::Symbol("Relu"))
    return new TGRelu(node, memTable);
  else if (symbol == onnx::Symbol("LRN"))
    return new TGLRN(node, memTable);
  else if (symbol == onnx::Symbol("MaxPool"))
    return new TGMaxPool(node, memTable);
  else if (symbol == onnx::Symbol("Gemm"))
    return new TGGemm(node, memTable);
  else if (symbol == onnx::Symbol("Softmax"))
    return new TGSoftmax(node, memTable);
  std::cerr << "unsupported node type: " << node.kind().toString() << std::endl;
  return nullptr;
}

void TGTargetLowering::LowerOperation(const onnx::Node &node,
                                      std::vector<std::unique_ptr<Operator> > &instList) {
  uint32_t symbol = node.kind();
  std::unique_ptr<Operator> oper(LowerHelper(node, m_globalMemLayout));
  // FIXME ignore unsupported operation
  if (nullptr == oper)
    return;
  std::cout << "lowering: " << oper->getName() << std::endl;
  instList.push_back(std::move(oper));
}
