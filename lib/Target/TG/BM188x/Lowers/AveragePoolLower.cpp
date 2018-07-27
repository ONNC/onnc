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
#include "AveragePoolLower.h"
#include "../Compute/MemOperand.h"

using namespace bm188x;
using namespace onnc;

//===----------------------------------------------------------------------===//
// AveragePoolLower
//===----------------------------------------------------------------------===//
onnc::ComputeOperator *AveragePoolLower::act(Module &pModule,
                                             ::onnx::Node &pNode) const
{
  /**
    auto *input = m_pBackend->getMemOperand(pNode.inputs()[0],
                                            MemType::NEURON);
    auto *output = m_pBackend->getMemOperand(pNode.outputs()[0],
                                             MemType::NEURON);
    auto *op = new BM188X::TGAveragePool(pNode);
    return op->addMemOperands(input, output);
   */
  return nullptr;
}

int AveragePoolLower::isMe(const ::onnx::Node &pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("AveragePool"))
    return 10;
  return 0;
}
