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
#define DEBUG_TYPE "tg_leakyrelu"
#include "TGLeakyRelu.h"
#include "BM188xCodeEmitter.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/TG/BM188x/bmkernel_api.h>

namespace onnc {
namespace BM188X {

TGLeakyRelu::TGLeakyRelu(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("LeakyRelu"))
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  if (inDim.size() == 4) {
    m_N = inDim[0].dim;
    m_C = inDim[1].dim;
    m_H = inDim[2].dim;
    m_W = inDim[3].dim;
  } else if (inDim.size() == 2) {
    m_N = inDim[0].dim;
    m_C = 1;
    m_H = inDim[1].dim;
    m_W = 1;
  } else {
    assert(0 && "inDim.size() != 4 & !=2");
  }

  m_Alpha = pNode.f(::onnx::Symbol("alpha"));
}

TGLeakyRelu *TGLeakyRelu::addMemOperands(MemOperand *pInput,
                                         MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pOutput);

  return this;
}

void TGLeakyRelu::emit() const
{
  bmnet::bmnet_asm::bmnet_leakyrelu_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr, // input_gaddr
      m_MemOperands[1]->m_Addr, // output_gaddr
      m_N,                      // input_n
      m_C,                      // input_c
      m_H,                      // input_h
      m_W,                      // input_w
      m_GTRShiftWidth,          // GT_right_shift_width
      m_LERShiftWidth,          // LE_right_shift_width
      m_GTScale,                // GT_scale
      m_LEScale);               // LE_scale
}

void TGLeakyRelu::update(
    const tg::bm1880::LayerCalibrationParameter *pLayerCtable)
{
  const auto &leakyrelu = pLayerCtable->leakyrelu_param();

  m_GTRShiftWidth = leakyrelu.gt_right_shift_width();
  m_LERShiftWidth = leakyrelu.le_right_shift_width();
  m_GTScale = leakyrelu.gt_scale();
  m_LEScale = leakyrelu.le_scale();
}

} // namespace BM188X
} // namespace onnc
