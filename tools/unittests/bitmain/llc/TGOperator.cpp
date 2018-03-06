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

// TGConv
TGConv::TGConv(const onnx::Node &node, uint64_t offset) : TGOperator(node, "Conv") {
  m_totalWeightSize = updateWeightSize(node, offset, m_weightOffset);
}

void TGConv::emit(void) const {
  std::cout << "TGConv::emit" << std::endl;
  bmnet_conv_forward_bmkernel(0,     // gaddr_t             ga_ifmap,
                              0,     // gaddr_t             ga_ofmap,
                              0,     // gaddr_t             ga_weight,
                              0,     // gaddr_t             ga_bias,
                              0,     // gaddr_t             ga_bn_mean,
                              0,     // gaddr_t             ga_bn_variance,
                              0,     // gaddr_t             ga_scale,
                              0,     // gaddr_t             ga_scale_bias,
                              4,     // int                 input_n,
                              3,     // int                 input_c,
                              50,    // int                 input_h,
                              50,    // int                 input_w,
                              1,     // int                 groups,
                              5,     // int                 output_c,
                              5,     // u16                 kh,
                              5,     // u16                 kw,
                              1,     // u16                 dilation_h,
                              1,     // u16                 dilation_w,
                              0,     // u8                  pad_h,
                              0,     // u8                  pad_w,
                              1,     // u8                  stride_h,
                              1,     // u8                  stride_w,
                              false, // int                 result_add,
                              0,     // int                 do_bias,
                              0,     // int                 do_bn,
                              0,     // int                 do_scale,
                              0,     // int                 do_scale_bias,
                              0,     // int                 do_activation,
                              0,     // float               bn_scale,
                              0,     // float               bn_eps,
                              0,     // int                 activation_method,
                              0,     // float               activation_arg[],
                              0,     // gaddr_t             activation_ga_slope,
                              0      //  bool activation_channel_shared
                              );
}

// TGRelu
TGRelu::TGRelu(const onnx::Node &node, uint64_t offset) : TGOperator(node, "Relu") {
}

void TGRelu::TGRelu::emit(void) const {}

// TGLRN
TGLRN::TGLRN(const onnx::Node &node, uint64_t offset) : TGOperator(node, "LRN") {
}

void TGLRN::TGLRN::emit(void) const {}

// TGMaxPool
TGMaxPool::TGMaxPool(const onnx::Node &node, uint64_t offset)
    : TGOperator(node, "MaxPool") {
}

void TGMaxPool::TGMaxPool::emit(void) const {}

// TGGemm
TGGemm::TGGemm(const onnx::Node &node, uint64_t offset) : TGOperator(node, "Gemm") {
  m_totalWeightSize = updateWeightSize(node, offset, m_weightOffset);
}

void TGGemm::TGGemm::emit(void) const {}

// TGSoftmax
TGSoftmax::TGSoftmax(const onnx::Node &node, uint64_t offset)
    : TGOperator(node, "Softmax") {
}

void TGSoftmax::TGSoftmax::emit(void) const {}
