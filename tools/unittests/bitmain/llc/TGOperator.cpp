#include "TGOperator.h"
#include "onnx/common/ir.h"
#include <bmkernel_api.h>
#include <cassert>
#include <iostream>
#include <unordered_set>

namespace {
int getSize(onnx::TensorProto_DataType type) {
  switch (type) {
  case onnx::TensorProto_DataType_FLOAT:
    return sizeof(float);
  case onnx::TensorProto_DataType_INT8:
    return sizeof(int8_t);
  case onnx::TensorProto_DataType_INT16:
    return sizeof(int16_t);
  case onnx::TensorProto_DataType_INT32:
    return sizeof(int32_t);
  default:
    assert(0 && "unimplemented type");
    return 0;
  }
}
}

TGOperator *TGOperator::makeTGOperator(const onnx::Node &node,
                                       uint64_t offset) {
  uint32_t symbol = node.kind();
  if (symbol == onnx::Symbol("Conv"))
    return new TGConv(node, offset);
  else if (symbol == onnx::Symbol("Relu"))
    return new TGRelu(node, offset);
  else if (symbol == onnx::Symbol("LRN"))
    return new TGLRN(node, offset);
  else if (symbol == onnx::Symbol("MaxPool"))
    return new TGMaxPool(node, offset);
  else if (symbol == onnx::Symbol("Gemm"))
    return new TGGemm(node, offset);
  else if (symbol == onnx::Symbol("Softmax"))
    return new TGSoftmax(node, offset);
  std::cerr << "unsupported node type: " << node.kind().toString() << std::endl;
  return nullptr;
}

TGOperator::TGOperator(const onnx::Node &node, const std::string &name)
    : m_totalWeightSize(0), m_name(name) {
}

uint64_t TGOperator::updateWeightSize(const onnx::Node &node, uint64_t offset,
                                      std::vector<uint64_t> &weightOffset) {

  int64_t totalWeightSize = 0;
  // walk around the lack of const member function in onnx::Graph
  auto *graph = const_cast<onnx::Graph *>(node.owningGraph());
  std::unordered_set<std::string> initNames(graph->initializer_names().begin(),
                                            graph->initializer_names().end());
  for (auto it = node.inputs().begin(), ie = node.inputs().end(); it != ie;
       ++it) {
    weightOffset.push_back(offset);
    const onnx::Value *val = *it;
    if (0 == val->sizes().size() || 0 == initNames.count(val->uniqueName()))
      continue;
    int64_t totalDim = 1;
    for (auto &dimension : val->sizes()) {
      totalDim *= dimension.dim;
    }
    int64_t sizeofType = getSize(val->elemType());
    offset += totalDim * sizeofType;
    totalWeightSize += totalDim * sizeofType;
  }
  return totalWeightSize;
}

// TGLRN
TGLRN::TGLRN(const onnx::Node &node, uint64_t offset) : TGOperator(node, "LRN") {
}

void TGLRN::TGLRN::emit(void) const {}

// TGMaxPool
TGMaxPool::TGMaxPool(const onnx::Node &node, uint64_t offset)
    : TGOperator(node, "MaxPool") {
}

void TGMaxPool::TGMaxPool::emit(void) const {}

// TGSoftmax
TGSoftmax::TGSoftmax(const onnx::Node &node, uint64_t offset)
    : TGOperator(node, "Softmax") {
}

void TGSoftmax::TGSoftmax::emit(void) const {}
