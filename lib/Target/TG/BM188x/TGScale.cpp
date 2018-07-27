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
#define DEBUG_TYPE "tg_scale"
#include "TGScale.h"
#include "BM188xCodeEmitter.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/TG/BM188x/bmkernel_api.h>

namespace onnc {
namespace BM188X {

TGScale::TGScale(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("Scale")), m_MScaleDim(0),
      m_MInnerDim(0), m_RShiftWidth(0)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_N = inDim[0].dim;
  m_C = inDim[1].dim;
  m_H = inDim[2].dim;
  m_W = inDim[3].dim;

  m_MScaleDim = m_C;
  m_MInnerDim = m_H * m_W;
}

TGScale *TGScale::addMemOperands(MemOperand *pInput, MemOperand *pScale,
                                 MemOperand *pBias, MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pScale);
  m_MemOperands.push_back(pBias);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TGScale::emit() const
{
  bmnet::bmnet_asm::bmnet_scale_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr, // input
      m_MemOperands[1]->m_Addr, // scale
      m_MemOperands[2]->m_Addr, // bias
      m_MemOperands[3]->m_Addr, // outpur,
      m_N, m_C, m_H, m_W, m_MScaleDim, m_MInnerDim, m_RShiftWidth);
}

void TGScale::update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable)
{
  m_RShiftWidth = pLayerCtable->right_shift_width();
}

} // namespace BM188X
} // namespace onnc
