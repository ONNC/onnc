//===- RNN.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/RNN.h>

using namespace onnc;

char RNN::ID = 0;

//===----------------------------------------------------------------------===//
// RNN
//===----------------------------------------------------------------------===//
RNN::RNN()
  : ComputeOperator("RNN", ID),
    m_ActivationAlpha(),
    m_ActivationBeta(),
    m_Activations(),
    m_Clip(),
    m_Direction(),
    m_HiddenSize(),
    m_OutputSequence() {
}

RNN::RNN(const FloatsAttr& pActivationAlpha,
         const FloatsAttr& pActivationBeta,
         const StringsAttr& pActivations,
         const FloatAttr& pClip,
         const StringAttr& pDirection,
         const IntAttr& pHiddenSize,
         const IntAttr& pOutputSequence)
  : ComputeOperator("RNN", ID),
    m_ActivationAlpha(pActivationAlpha),
    m_ActivationBeta(pActivationBeta),
    m_Activations(pActivations),
    m_Clip(pClip),
    m_Direction(pDirection),
    m_HiddenSize(pHiddenSize),
    m_OutputSequence(pOutputSequence) {
}

void RNN::print(std::ostream& pOS) const
{
}

bool RNN::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
