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
#define DEBUG_TYPE "tg_gemm"

#include "TGGemm.h"
#include "BM188xCodeEmitter.h"
#include "PatternMatch.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/Sophon/BM188x/bmkernel_api.h>

namespace pm = onnc::PatternMatch;

namespace onnc {
namespace BM188X {

// TGGemm
// Y = alpha * A * B + beta * C
// where input tensor A has dimension (M X K) , input tensor B has dimension (K
// X N), input tensor C and output tensor Y have dimension (M X N).
TGGemm::TGGemm(const xNode &pNode)
    : BM188xComputeOperator(pNode, std::string("Gemm")), m_InRowNum(0),
      m_InColNum(0), m_OutColNum(0), m_HaveBias(0), m_WeightTp(false),
      m_EnableRelu(false), m_RShiftWidth(0)
{

  const std::vector< xDimension> aDim = pNode.inputs()[0]->sizes();
  const std::vector< xDimension> bDim = pNode.outputs()[0]->sizes();
  m_InRowNum = aDim[0].dim;
  m_InColNum = aDim[1].dim;
  if (aDim.size() == 4) {
    m_InColNum *= aDim[2].dim * aDim[3].dim;
  }
  m_OutColNum = bDim[1].dim;
  m_HaveBias = true;

  if (pNode.hasAttribute(xSymbol("transB"))) {
    auto transB = pNode.i(xSymbol("transB"));
    DEBUG(dbgs() << "transB:" << transB << std::endl;);
    m_WeightTp = true;
  }

  m_EnableRelu = pm::match(&pNode, pm::mTrueAttr("do_relu"));
}

TGGemm *TGGemm::addMemOperands(MemOperand *pInput, MemOperand *pOutput,
                               MemOperand *pWeight, MemOperand *pBias)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pWeight);
  m_MemOperands.push_back(pBias);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TGGemm::emit() const
{
  int do_activation = m_EnableRelu;
  int activation_method = RELU;

  bmnet::bmnet_asm::bmnet_fc_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr,        // input_data_gaddr
      m_MemOperands[1]->m_Addr,        // weight_data_gaddr
      m_MemOperands[2]->m_Addr,        // bias_data_gaddr
      m_MemOperands[3]->m_Addr,        // output_data_gaddr
      m_InRowNum,                      // input_row_num
      m_InColNum,                      // input_col_num
      m_OutColNum,                     // weight_col_num
      m_HaveBias,                      // have_bias
      do_activation,                   // do_activation
      activation_method,               // activation_method
      bmnet::bmnet_asm::GADDR_INVALID, // activation_ga_slope
      0,                               // activation_channel_shared
      0,                               // activation_gt_scale
      0,                               // activation_gt_rshift
      0,                               // activation_le_scale
      0,                               // activation_le_rshift
      m_WeightTp,                      // weight_transpose
      0,                               // left_shift_width //TODO
      m_RShiftWidth                    // right_shift_width
  );
}

void TGGemm::update(const tg::bm1880::LayerCalibrationParameter *pLayerCtable)
{
  m_RShiftWidth = pLayerCtable->right_shift_width();
}

} // namespace BM188X
} // namespace onnc
