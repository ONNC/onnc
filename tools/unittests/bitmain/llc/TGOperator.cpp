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

TGOperator::TGOperator(const onnx::Node &node, const std::string &name)
    : m_name(name) {}

