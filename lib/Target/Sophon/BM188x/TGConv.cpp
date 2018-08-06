//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#include "TGConv.h"
#include "BM188xCodeEmitter.h"
#include "PatternMatch.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/Sophon/BM188x/bmkernel_api.h>

#define DEBUG_TYPE "tg_conv"

namespace pm = onnc::PatternMatch;

namespace onnc {
namespace BM188X {

// TGConv
TGConv::TGConv(const xNode &pNode)
    : BM188xComputeOperator(pNode, std::string("Conv")), m_Groups(1),
      m_DilationH(1), m_DilationW(1), m_PadH(0), m_PadW(0), m_StrideH(1),
      m_StrideW(1), m_DoBias(0), /* m_DoRelu(0),*/ m_DoScale(0),
      m_DoScaleBias(0), m_RShiftWidth(0), m_ScaleRShiftWidth(0),
      m_ConvOutputThreshold(0)
{
  const std::vector<xDimension> inDim = pNode.inputs()[0]->sizes();
  m_InN = inDim[0].dim;
  m_InC = inDim[1].dim;
  m_InH = inDim[2].dim;
  m_InW = inDim[3].dim;
  const std::vector<xDimension> weightDim = pNode.inputs()[1]->sizes();
  m_OutC = weightDim[0].dim;
  if (pNode.hasAttribute(xSymbol("group"))) {
    m_Groups = pNode.i(xSymbol("group"));
  }
  if (pNode.hasAttribute(xSymbol("kernel_shape"))) {
    auto &i = pNode.is(xSymbol("kernel_shape"));
    m_KH = i[0];
    m_KW = i[1];
  }
  if (pNode.hasAttribute(xSymbol("dilations"))) {
    auto &i = pNode.is(xSymbol("dilations"));
    m_DilationH = i[0];
    m_DilationW = i[1];
  }
  // [leftPad, downPad, rightPad, upPad]
  if (pNode.hasAttribute(xSymbol("pads"))) {
    auto &i = pNode.is(xSymbol("pads"));
    // NOTE: It is for bmkernel only padding on both ends
    m_PadH = i[0];
    m_PadW = i[1];
  }
  if (pNode.hasAttribute(xSymbol("strides"))) {
    auto &i = pNode.is(xSymbol("strides"));
    m_StrideH = i[0];
    m_StrideW = i[1];
  }

  if (pm::match(m_pNode, pm::mTrueAttr("do_scale"))) {
    m_DoScale = 1;
  }

  if (pm::match(m_pNode, pm::mTrueAttr("do_scale_bias"))) {
    m_DoScaleBias = 1;
  }

  int idx = 3;
  if (pNode.inputs().size() - m_DoScale - m_DoScaleBias == 3) {
    m_DoBias = 1;
    m_BiasIdx = idx++;
  }
  if (m_DoScale) {
    m_ScaleIdx = idx++;
    m_ConvOutputThreshold = pNode.f(xSymbol("conv_output_threshold"));
  }
  if (m_DoScaleBias)
    m_ScaleBiasIdx = idx++;
}

TGConv *TGConv::addMemOperands(MemOperand *pInput, MemOperand *pOutput,
                               MemOperand *pWeight, MemOperand *pBias,
                               MemOperand *pScale, MemOperand *pScaleBias)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pWeight);
  m_MemOperands.push_back(pOutput);
  if (pBias != nullptr) {
    assert(m_DoBias);
    m_MemOperands.push_back(pBias);
  }
  if (pScale != nullptr) {
    assert(m_DoScale);
    m_MemOperands.push_back(pScale);
  }
  if (pScaleBias != nullptr) {
    assert(m_DoScaleBias);
    m_MemOperands.push_back(pScaleBias);
  }
  return this;
}

void TGConv::emit() const
{
  float activation_arg[1] = { 0.0f };

  uint64_t biasAddr = m_DoBias ? m_MemOperands[m_BiasIdx]->m_Addr
                               : bmnet::bmnet_asm::GADDR_INVALID;
  uint64_t scale_addr = m_DoScale ? m_MemOperands[m_ScaleIdx]->m_Addr
                                  : bmnet::bmnet_asm::GADDR_INVALID;
  uint64_t scale_bias_addr = m_DoScaleBias
                                 ? m_MemOperands[m_ScaleBiasIdx]->m_Addr
                                 : bmnet::bmnet_asm::GADDR_INVALID;
  bmnet::bmnet_asm::bmnet_conv_parallel_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr,        // ifmap
      m_MemOperands[2]->m_Addr,        // ofmap
      m_MemOperands[1]->m_Addr,        // weight
      biasAddr,                        // bias
      bmnet::bmnet_asm::GADDR_INVALID, // ga_bn_mean,
      bmnet::bmnet_asm::GADDR_INVALID, // ga_bn_variance,
      scale_addr,                      // ga_scale,
      scale_bias_addr,                 // ga_scale_bias,
      m_InN, m_InC, m_InH, m_InW, m_Groups, m_OutC, m_KH, m_KW, m_DilationH,
      m_DilationW, m_PadH, m_PadW, m_StrideH, m_StrideW, false, // result_add
      m_DoBias,                                                 // do_bias,
      0,                                                        // do_bn,
      m_DoScale, m_DoScaleBias, pm::match(m_pNode, pm::mTrueAttr("do_relu")),
      0,                  // bn_scale,
      0,                  // bn_eps,
      0,                  // activation_method,
      activation_arg,     // activation_arg[],
      0,                  // activation_ga_slope,
      0,                  // activation_channel_shared
      0,                  // activation_gt_rshift
      0,                  // activation_gt_rshift
      0,                  // activation_le_scale
      0,                  // activation_le_rshift
      m_RShiftWidth,      // right_shift_width
      0,                  // bn_right_shift_width
      m_ScaleRShiftWidth, // scale_right_shift_width
      0                   // use_winograd
  );
}

void TGConv::update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable)
{
  m_RShiftWidth = pLayerCtable->right_shift_width();
  m_ScaleRShiftWidth =
      pLayerCtable->convolution_param().scale_right_shift_width();
  // TODO add prelu_param
}

} // namespace BM188X
} // namespace onnc
