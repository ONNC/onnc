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
#include "TLPool.h"
#include "BM188xCodeEmitter.h"
#include "PatternMatch.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/Sophon/BM188x/bmkernel_api.h>

#define DEBUG_TYPE "tl_pool"

namespace onnc {
namespace BM188X {

TLPool::TLPool(const xNode &pNode)
    : BM188xComputeOperator(pNode, std::string("TLPool"))
{
  // Default value
  m_PadHTop = m_PadHBot = m_PadWLeft = m_PadWRight = 0;
  m_StrideH = m_StrideW = 1;
  m_RShiftWidth = 0;
  m_ThresholdXQuantized = 0;

  // ONNC extension attribute
  assert(pNode.hasAttribute(xSymbol("op_name")));
  m_SplitName = pNode.s(xSymbol("op_name"));

  assert(pNode.hasAttribute(xSymbol("input_dim")));
  assert(pNode.hasAttribute(xSymbol("output_dim")));

  auto &inDim = pNode.is(xSymbol("input_dim"));
  m_InN = inDim[0];
  m_InC = inDim[1];
  m_InH = inDim[2];
  m_InW = inDim[3];

  auto &outDim = pNode.is(xSymbol("output_dim"));
  m_OutN = outDim[0];
  m_OutC = outDim[1];
  m_OutH = outDim[2];
  m_OutW = outDim[3];

  assert(pNode.hasAttribute(xSymbol("ifmap_laddr")));
  assert(pNode.hasAttribute(xSymbol("ofmap_laddr")));

  m_IFmapAddr = pNode.i(xSymbol("ifmap_laddr"));
  m_OFmapAddr = pNode.i(xSymbol("ofmap_laddr"));

  assert(pNode.hasAttribute(xSymbol("is_avg_pooling")));
  m_IsAvgPooling = pNode.i(xSymbol("is_avg_pooling"));
  // End extension

  if (pNode.hasAttribute(xSymbol("kernel_shape"))) {
    auto &i = pNode.is(xSymbol("kernel_shape"));
    assert(i.size() == 2);
    m_KH = i[0];
    m_KW = i[1];
  }

  // [upPad, leftPad, downPad, rightPad]
  if (pNode.hasAttribute(xSymbol("slice_pads"))) {
    auto &i = pNode.is(xSymbol("slice_pads"));
    m_PadHTop = i[0];
    m_PadWLeft = i[1];
    m_PadHBot = i[2];
    m_PadWRight = i[3];
  }

  if (pNode.hasAttribute(xSymbol("strides"))) {
    auto &i = pNode.is(xSymbol("strides"));
    m_StrideH = i[0];
    m_StrideW = i[1];
  }
}

TLPool *TLPool::addMemOperands(MemOperand *pInput, MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TLPool::emit() const
{
  bmnet::bmnet_asm::asm_context::get_context().name = m_SplitName;
  bmnet::bmnet_asm::bmnet_tl_pooling_forward_bmkernel(
      m_IFmapAddr, // ifmap
      m_OFmapAddr, // ofmap
      m_InN, m_InC, m_InH, m_InW, m_OutN, m_OutC, m_OutH, m_OutW, m_KH, m_KW,
      m_StrideH, m_StrideW,                          // stride
      m_PadHTop, m_PadHBot, m_PadWLeft, m_PadWRight, // padding
      m_IsAvgPooling,                                // is_avg_pooling
      m_RShiftWidth, m_ThresholdXQuantized);
}

void TLPool::update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable)
{
  m_RShiftWidth = pLayerCtable->right_shift_width();
  m_ThresholdXQuantized = *pLayerCtable->threshold_x_quantized().data();
}

} // namespace BM188X
} // namespace onnc
