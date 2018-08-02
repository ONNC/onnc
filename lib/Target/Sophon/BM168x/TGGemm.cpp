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
#include "TGGemm.h"
#include "BM168xCodeEmitter.h"
#include <onnc/Target/Sophon/BM168x/bmkernel_api.h>

#define DEBUG_TYPE "tg_gemm"
#include <onnc/Support/Debug.h>

using namespace onnc;

// TGGemm
// Y = alpha * A * B + beta * C
// where input tensor A has dimension (M X K) , input tensor B has dimension (K
// X N), input tensor C and output tensor Y have dimension (M X N).
TGGemm::TGGemm(const ::onnx::Node &pNode)
    : ComputeOperator2(pNode, "Gemm"), m_InRowNum(0), m_InColNum(0),
      m_OutColNum(0), m_HaveBias(0), m_UsingRelu(0), m_WeightTp(false)
{
  // auto inputs = pNode.inputs();
  // auto outputs = pNode.outputs();

  //// input
  // m_MemOperands.push_back(MemOperand(inputs[0]->uniqueName(),
  //                                   inputs[0]->sizes(),
  //                                   inputs[0]->elemType(), MemType::NEURON));
  //// weight
  // m_MemOperands.push_back(MemOperand(inputs[1]->uniqueName(),
  //                                   inputs[1]->sizes(),
  //                                   inputs[1]->elemType(), MemType::WEIGHT));
  //// bias
  // m_MemOperands.push_back(MemOperand(inputs[2]->uniqueName(),
  //                                   inputs[2]->sizes(),
  //                                   inputs[2]->elemType(), MemType::WEIGHT));
  //// output
  // m_MemOperands.push_back(
  //    MemOperand(outputs[0]->uniqueName(), outputs[0]->sizes(),
  //               outputs[0]->elemType(), MemType::NEURON));

  const std::vector< ::onnx::Dimension> aDim = pNode.inputs()[0]->sizes();
  const std::vector< ::onnx::Dimension> bDim = pNode.outputs()[0]->sizes();
  m_InRowNum = aDim[0].dim;
  m_InColNum = aDim[1].dim;
  if (aDim.size() == 4) {
    m_InColNum *= aDim[2].dim * aDim[3].dim;
  }
  m_OutColNum = bDim[1].dim;
  m_HaveBias = true;
  m_UsingRelu = false;

  if (pNode.hasAttribute(::onnx::Symbol("transB"))) {
    auto transB = pNode.i(::onnx::Symbol("transB"));
    DEBUG(dbgs() << "transB:" << transB << std::endl;);
    m_WeightTp = true;
  }
}

void TGGemm::emit() const
{
  DEBUG(dbgs() << "TGGemm::emit\tm_inputAddr:" << m_MemOperands[0]->m_Addr
               << " m_WeightAddr:" << m_MemOperands[1]->m_Addr
               << " m_BiasAddr:" << m_MemOperands[2]->m_Addr << " m_OutputAddr:"
               << m_MemOperands[3]->m_Addr << " m_InRowNum:" << m_InRowNum
               << " m_InColNum:" << m_InColNum << " m_OutColNum:" << m_OutColNum
               << " m_HaveBias:" << m_HaveBias << " m_UsingRelu:" << m_UsingRelu
               << " m_WeightTp:" << m_WeightTp << std::endl;);
  bmnet::bmnet_asm::bmnet_fc_forward_bmkernel(
      m_MemOperands[0]->m_Addr, m_MemOperands[1]->m_Addr,
      m_MemOperands[2]->m_Addr, m_MemOperands[3]->m_Addr, m_InRowNum,
      m_InColNum, m_OutColNum, m_HaveBias, m_UsingRelu, m_WeightTp);
}
