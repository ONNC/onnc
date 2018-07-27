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
#include "TGLRN.h"
#include "BM188xCodeEmitter.h"
#include <onnc/Target/TG/BM188x/bmkernel_api.h>

namespace onnc {
namespace BM188X {

TGLRN::TGLRN(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("LRN")), m_K(1)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_N = inDim[0].dim;
  m_C = inDim[1].dim;
  m_H = inDim[2].dim;
  m_W = inDim[3].dim;

  m_Alpha = pNode.f(::onnx::Symbol("alpha"));
  m_Beta = pNode.f(::onnx::Symbol("beta"));
  if (pNode.hasAttribute(::onnx::Symbol("bias"))) {
    m_K = pNode.f(::onnx::Symbol("bias"));
  }
  m_LocalSize = pNode.i(::onnx::Symbol("size"));
}

TGLRN *TGLRN::addMemOperands(MemOperand *pInput, MemOperand *pSquLut,
                             MemOperand *pPowerLut, MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pSquLut);
  m_MemOperands.push_back(pPowerLut);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TGLRN::emit() const
{
  bmnet::bmnet_asm::bmnet_lrn_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr, // input
      m_MemOperands[3]->m_Addr, // output
      m_MemOperands[1]->m_Addr, // sqr_lut
      m_MemOperands[2]->m_Addr, // power_lut,
      m_N, m_C, m_H, m_W, m_LocalSize, m_SumRightShiftWidth,
      m_LrnRightShiftWidth, m_ThresholdXQuantized);
}

void TGLRN::update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable)
{
  for (int i = 0; i < pLayerCtable->blob_param_size(); ++i) {
    std::string outputName = m_MemOperands[3]->m_Name;
    if (pLayerCtable->blob_param(i).name() == outputName) {
      m_LrnRightShiftWidth = pLayerCtable->blob_param(i).right_shift_width();
    } else if (pLayerCtable->blob_param(i).name() == "sum_sq") {
      m_SumRightShiftWidth = pLayerCtable->blob_param(i).right_shift_width();
    }
  }
  m_ThresholdXQuantized[0] = pLayerCtable->threshold_x_quantized(0);
  m_ThresholdXQuantized[1] = pLayerCtable->threshold_x_quantized(1);
}

} // namespace BM188X
} // namespace onnc
