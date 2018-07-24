//===- AveragePoolLower.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
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
