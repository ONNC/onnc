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
#include "BM168xCodeEmitter.h"
#include <onnc/Target/Sophon/BM168x/bmkernel_api.h>

using namespace onnc;

TGLRN::TGLRN(const xNode &pNode) : ComputeOperator2(pNode, "LRN"), m_K(1)
{

  // auto inputs = pNode.inputs();
  // auto outputs = pNode.outputs();

  //// input
  // m_MemOperands.push_back(MemOperand(inputs[0]->uniqueName(),
  //                                   inputs[0]->sizes(),
  //                                   inputs[0]->elemType(), MemType::NEURON));
  //// output
  // m_MemOperands.push_back(
  //    MemOperand(outputs[0]->uniqueName(), outputs[0]->sizes(),
  //               outputs[0]->elemType(), MemType::NEURON));

  const std::vector< xDimension> inDim = pNode.inputs()[0]->sizes();
  m_N = inDim[0].dim;
  m_C = inDim[1].dim;
  m_H = inDim[2].dim;
  m_W = inDim[3].dim;

  m_Alpha = pNode.f(xSymbol("alpha"));
  m_Beta = pNode.f(xSymbol("beta"));
  if (pNode.hasAttribute(xSymbol("bias"))) {
    m_K = pNode.f(xSymbol("bias"));
  }
  m_LocalSize = pNode.i(xSymbol("size"));
}

void TGLRN::TGLRN::emit() const
{
  std::cout << "TGLRUN::emit\tm_outputAddr:" << m_MemOperands[0]->m_Addr
            << " m_InputAddr:" << m_MemOperands[1]->m_Addr << " m_N:" << m_N
            << " m_C:" << m_C << " m_H:" << m_H << " m_W:" << m_W
            << " m_Alpha:" << m_Alpha << "m_LocalSize:" << m_LocalSize
            << " m_Beta:" << m_Beta << "m_K:" << m_K << std::endl;
  bmnet::bmnet_asm::bmnet_lrn_forward_bmkernel(
      m_MemOperands[0]->m_Addr, m_MemOperands[1]->m_Addr, m_N, m_C, m_H, m_W,
      m_Alpha, m_LocalSize, m_Beta, m_K);
}
