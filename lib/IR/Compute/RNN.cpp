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
    m_Direction("forward"),
    m_HiddenSize() {
}

RNN::RNN(const FloatsAttr& pActivationAlpha,
         const FloatsAttr& pActivationBeta,
         const StringsAttr& pActivations,
         const FloatAttr& pClip,
         const StringAttr& pDirection,
         const IntAttr& pHiddenSize)
  : ComputeOperator("RNN", ID),
    m_ActivationAlpha(pActivationAlpha),
    m_ActivationBeta(pActivationBeta),
    m_Activations(pActivations),
    m_Clip(pClip),
    m_Direction(pDirection),
    m_HiddenSize(pHiddenSize) {
}

RNN::RNN(const RNN& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_ActivationAlpha(pCopy.getActivationAlpha()),
    m_ActivationBeta(pCopy.getActivationBeta()),
    m_Activations(pCopy.getActivations()),
    m_Clip(pCopy.getClip()),
    m_Direction(pCopy.getDirection()),
    m_HiddenSize(pCopy.getHiddenSize()) {
}

void RNN::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getActivationAlpha() << ", " << getActivationBeta() << ", " << getActivations() << ", " << getClip() << ", " << getDirection() << ", " << getHiddenSize() << ">";
}

bool RNN::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
