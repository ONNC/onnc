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
#include "TGRelu.h"
#include "BM168xCodeEmitter.h"
#include <onnc/Target/Sophon/BM168x/bmkernel_api.h>

using namespace onnc;

TGRelu::TGRelu(const ::onnx::Node &pNode)
    : ComputeOperator2(pNode, "Relu"), m_NegativeSlope(0)
{
  const std::vector< ::onnx::Dimension> inDim = pNode.inputs()[0]->sizes();

  // auto inputs = pNode.inputs();
  // auto outputs = pNode.outputs();

  //// input
  // m_MemOperands.push_back(MemOperand(inputs[0]->uniqueName(),
  //                                   inputs[0]->sizes(),
  //                                   inputs[0]->elemType(), MemType::NEURON));
  //// output
  // m_MemOperands.push_back(MemOperand(outputs[0]->uniqueName(),
  //                                   outputs[0]->sizes(),
  //                                   outputs[0]->elemType(),
  //                                   MemType::NEURON));

  if (inDim.size() == 4) {
    m_N = inDim[0].dim;
    m_C = inDim[1].dim;
    m_H = inDim[2].dim;
    m_W = inDim[3].dim;
  } else if (inDim.size() == 2) {
    m_N = 1;
    m_C = 1;
    m_H = inDim[0].dim;
    m_W = inDim[1].dim;
  } else {
    assert(0 && "inDim.size() != 4 & !=2");
  }
}

void TGRelu::emit() const
{
  std::cout << "TGRelu::emit\tm_inputAddr:" << m_MemOperands[0]->m_Addr
            << " m_OutputAddr:" << m_MemOperands[1]->m_Addr
            << " m_NegativeSlope:" << m_NegativeSlope << " m_N:" << m_N
            << " m_C:" << m_C << " m_H:" << m_H << " m_W:" << m_W << std::endl;
  bmnet::bmnet_asm::bmnet_relu_forward_bmkernel(
      m_MemOperands[0]->m_Addr, m_MemOperands[1]->m_Addr, m_NegativeSlope, m_N,
      m_C, m_H, m_W);
}
