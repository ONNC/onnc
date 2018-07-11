#include "TGConv.h"
#include "BM168xCodeEmitter.h"
#include <bmnet/targets/plat-bm168x/bmkernel/bmkernel_api.h>

#define DEBUG_TYPE "tg_conv"
#include <onnc/Support/Debug.h>

using namespace onnc;

// TGConv
TGConv::TGConv(const ::onnx::Node &pNode)
    : ComputeOperator2(pNode, "Conv"), m_Groups(1), m_DilationH(1),
      m_DilationW(1), m_PadH(0), m_PadW(0), m_StrideH(1), m_StrideW(1),
      m_DoBias(0)
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
  m_InN = inDim[0].dim;
  m_InC = inDim[1].dim;
  m_InH = inDim[2].dim;
  m_InW = inDim[3].dim;
  const std::vector< ::onnx::Dimension> weightDim = pNode.inputs()[1]->sizes();
  m_OutC = weightDim[0].dim;
  if (pNode.hasAttribute(::onnx::Symbol("group"))) {
    m_Groups = pNode.i(::onnx::Symbol("group"));
  }
  if (pNode.hasAttribute(::onnx::Symbol("kernel_shape"))) {
    auto &i = pNode.is(::onnx::Symbol("kernel_shape"));
    m_KH = i[0];
    m_KW = i[1];
  }
  if (pNode.hasAttribute(::onnx::Symbol("dilations"))) {
    auto &i = pNode.is(::onnx::Symbol("dilations"));
    m_DilationH = i[0];
    m_DilationW = i[1];
  }
  // [leftPad, downPad, rightPad, upPad]
  if (pNode.hasAttribute(::onnx::Symbol("pads"))) {
    auto &i = pNode.is(::onnx::Symbol("pads"));
    // NOTE: It is for bmkernel only padding on both ends
    m_PadH = i[0];
    m_PadW = i[1];
  }
  if (pNode.hasAttribute(::onnx::Symbol("strides"))) {
    auto &i = pNode.is(::onnx::Symbol("strides"));
    m_StrideH = i[0];
    m_StrideW = i[1];
  }
  if (3 == pNode.inputs().size()) {
    m_DoBias = 1;
  }
}

void TGConv::emit() const
{
  DEBUG(dbgs() << "TGConv::emit\tm_ifmapAddr:" << m_MemOperands[0]->m_Addr
               << " m_OfmapAddr:" << m_MemOperands[2]->m_Addr
               << " m_WeightAddr:" << m_MemOperands[1]->m_Addr
               << " m_BiasAddr:" << m_MemOperands[3]->m_Addr
               << " m_InN:" << m_InN << " m_InC:" << m_InC << " m_InH:" << m_InH
               << " m_InW:" << m_InW << " m_OutC:" << m_OutC
               << " m_Groups:" << m_Groups << " m_KH:" << m_KH
               << " m_KW:" << m_KW << " m_DilationH:" << m_DilationH
               << " m_DilationW:" << m_DilationW << " m_PadH:" << (int)m_PadH
               << " m_PadW:" << (int)m_PadW << " m_StrideH:" << (int)m_StrideH
               << " m_StrideW:" << (int)m_StrideW << " m_DoBias:" << m_DoBias
               << std::endl;);
  bmnet::bmnet_conv_forward_bmkernel(
      *bm168x_kernel::getInstance().m_CTX, m_MemOperands[0]->m_Addr,
      m_MemOperands[2]->m_Addr, m_MemOperands[1]->m_Addr,
      m_MemOperands[3]->m_Addr,
      GADDR_INVALID, // ga_bn_mean,
      GADDR_INVALID, // ga_bn_variance,
      GADDR_INVALID, // ga_scale,
      GADDR_INVALID, // ga_scale_bias,
      m_InN, m_InC, m_InH, m_InW, m_Groups, m_OutC, m_KH, m_KW, m_DilationH,
      m_DilationW, m_PadH, m_PadW, m_StrideH, m_StrideW, false, // result_add
      m_DoBias,                                                 // do_bias,
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
