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
#define DEBUG_TYPE "tg_globalaveragepool"
#include "TGGlobalAveragePool.h"
#include "BM188xCodeEmitter.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/Sophon/BM188x/bmkernel_api.h>

namespace onnc {
namespace BM188X {

TGGlobalAveragePool::TGGlobalAveragePool(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("AveragePool")), m_EnableRelu(0),
      m_RShiftWidth(0), m_ThresholdXQuantized(0)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();
  m_N = inDim[0].dim;
  m_C = inDim[1].dim;
  m_H = inDim[2].dim;
  m_W = inDim[3].dim;

  if (pNode.hasAttribute(::onnx::Symbol("enable_relu"))) {
    m_EnableRelu = pNode.i(::onnx::Symbol("enable_relu"));
  }
}

TGGlobalAveragePool *TGGlobalAveragePool::addMemOperands(MemOperand *pInput,
                                                         MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TGGlobalAveragePool::emit() const
{

  bmnet::bmnet_asm::bmnet_pooling_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr,        // ifmap_gaddr
      m_MemOperands[1]->m_Addr,        // ofmap_gaddr
      bmnet::bmnet_asm::GADDR_INVALID, // index_gaddr
      bmnet::bmnet_asm::GADDR_INVALID, // o_findex_gaddr
      m_N, m_C, m_H, m_W, m_H, m_W, 0, 0, 0, 0, 1, 1,
      1,                      // is_avg_pooling
      0.0f,                   // avg_const
      m_EnableRelu,           // do_relu
      m_RShiftWidth,          // right_shift_width
      &m_ThresholdXQuantized, // threshold_x_quantized
      0                       // ceil_mode
  );
}

void TGGlobalAveragePool::update(
    const tg::bm1880::LayerCalibrationParameter *pLayerCtable)
{
  m_RShiftWidth = pLayerCtable->right_shift_width();
  m_ThresholdXQuantized = *pLayerCtable->threshold_x_quantized().data();
}

} // namespace BM188X
} // namespace onnc
