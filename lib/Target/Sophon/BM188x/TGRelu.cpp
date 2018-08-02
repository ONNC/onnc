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
#define DEBUG_TYPE "tg_relu"
#include "TGRelu.h"
#include "BM188xCodeEmitter.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/Sophon/BM188x/bmkernel_api.h>

namespace onnc {
namespace BM188X {

TGRelu::TGRelu(const ::onnx::Node &pNode)
    : BM188xComputeOperator(pNode, std::string("Relu")), m_NegativeSlope(0)
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
}

TGRelu *TGRelu::addMemOperands(MemOperand *pInput, MemOperand *pOutput)
{
  m_MemOperands.push_back(pInput);
  m_MemOperands.push_back(pOutput);
  return this;
}

void TGRelu::emit() const
{

  bmnet::bmnet_asm::bmnet_relu_fixed_forward_bmkernel(
      m_MemOperands[0]->m_Addr, // input_gaddr
      m_MemOperands[1]->m_Addr, // output_gaddr
      m_NegativeSlope,          // negative_slope
      m_N,                      // input_n
      m_C,                      // input_c
      m_H,                      // input_h
      m_W                       // input_w
  );
}

} // namespace BM188X
} // namespace onnc
