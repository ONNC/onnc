#include "TGConv.h"
#include "BM168xCodeEmitter.h"
#include <bmkernel_api.h>

#define DEBUG_TYPE "tg_conv"
#include <onnc/Support/Debug.h>

using namespace onnc;
using namespace std;

// TGConv
TGConv::TGConv(const ::onnx::Node &pNode)
    : ComputeOperator2(pNode, "Conv"), m_groups(1), m_dilationH(1),
      m_dilationW(1), m_padH(0), m_padW(0), m_strideH(1), m_strideW(1),
      m_doBias(0)
{
  // auto inputs = pNode.inputs();
  // auto outputs = pNode.outputs();

  //// ifmap
  // m_MemOperands.push_back(MemOperand(inputs[0]->uniqueName(),
  //                                   inputs[0]->sizes(),
  //                                   inputs[0]->elemType(),
  //                                   GLOBAL_NEURON_TAG));
  //// weight
  // m_MemOperands.push_back(MemOperand(inputs[1]->uniqueName(),
  //                                   inputs[1]->sizes(),
  //                                   inputs[1]->elemType(),
  //                                   GLOBAL_WEIGHT_TAG));
  //// bias
  // m_MemOperands.push_back(MemOperand(inputs[2]->uniqueName(),
  //                                   inputs[2]->sizes(),
  //                                   inputs[2]->elemType(),
  //                                   GLOBAL_WEIGHT_TAG));
  //// ofmap
  // m_MemOperands.push_back(
  //    MemOperand(outputs[0]->uniqueName(), outputs[0]->sizes(),
  //               outputs[0]->elemType(), GLOBAL_NEURON_TAG));

  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_inN = inDim[0].dim;
  m_inC = inDim[1].dim;
  m_inH = inDim[2].dim;
  m_inW = inDim[3].dim;
  const std::vector< ::onnx::Dimension> weightDim = pNode.inputs()[1]->sizes();
  m_outC = weightDim[0].dim;
  if (pNode.hasAttribute(::onnx::Symbol("group"))) {
    m_groups = pNode.i(::onnx::Symbol("group"));
  }
  if (pNode.hasAttribute(::onnx::Symbol("kernel_shape"))) {
    auto &i = pNode.is(::onnx::Symbol("kernel_shape"));
    m_kH = i[0];
    m_kW = i[1];
  }
  if (pNode.hasAttribute(::onnx::Symbol("dilations"))) {
    auto &i = pNode.is(::onnx::Symbol("dilations"));
    m_dilationH = i[0];
    m_dilationW = i[1];
  }
  // [leftPad, downPad, rightPad, upPad]
  if (pNode.hasAttribute(::onnx::Symbol("pads"))) {
    auto &i = pNode.is(::onnx::Symbol("pads"));
    // NOTE: It is for bmkernel only padding on both ends
    m_padH = i[0];
    m_padW = i[1];
  }
  if (pNode.hasAttribute(::onnx::Symbol("strides"))) {
    auto &i = pNode.is(::onnx::Symbol("strides"));
    m_strideH = i[0];
    m_strideW = i[1];
  }
  if (3 == pNode.inputs().size()) {
    m_doBias = 1;
  }
}

void TGConv::emit() const
{
  DEBUG(dbgs() << "TGConv::emit\tm_ifmapAddr:" << m_MemOperands[0]->addr
               << " m_ofmapAddr:" << m_MemOperands[2]->addr
               << " m_weightAddr:" << m_MemOperands[1]->addr
               << " m_biasAddr:" << m_MemOperands[3]->addr << " m_inN:" << m_inN
               << " m_inC:" << m_inC << " m_inH:" << m_inH << " m_inW:" << m_inW
               << " m_outC:" << m_outC << " m_groups:" << m_groups << " m_kH:"
               << m_kH << " m_kW:" << m_kW << " m_dilationH:" << m_dilationH
               << " m_dilationW:" << m_dilationW << " m_padH:" << (int)m_padH
               << " m_padW:" << (int)m_padW << " m_strideH:" << (int)m_strideH
               << " m_strideW:" << (int)m_strideW << " m_doBias:" << m_doBias
               << std::endl;);
  bmnet::bmnet_conv_forward_bmkernel(
      *bm168x_kernel::getInstance().ctx, m_MemOperands[0]->addr,
      m_MemOperands[2]->addr, m_MemOperands[1]->addr, m_MemOperands[3]->addr,
      GADDR_INVALID, // ga_bn_mean,
      GADDR_INVALID, // ga_bn_variance,
      GADDR_INVALID, // ga_scale,
      GADDR_INVALID, // ga_scale_bias,
      m_inN, m_inC, m_inH, m_inW, m_groups, m_outC, m_kH, m_kW, m_dilationH,
      m_dilationW, m_padH, m_padW, m_strideH, m_strideW, false, // result_add
      m_doBias,                                                 // do_bias,
      0,                                                        // do_bn,
      0,                                                        // do_scale,
      0,       // do_scale_bias,
      0,       // do_activation,
      0,       // bn_scale,
      0,       // bn_eps,
      0,       // activation_method,
      nullptr, // activation_arg[],
      0,       // activation_ga_slope,
      0        // activation_channel_shared
  );
}
