//===- LSTM.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/LSTM.h>

using namespace onnc;

char LSTM::ID = 0;

//===----------------------------------------------------------------------===//
// LSTM
//===----------------------------------------------------------------------===//
LSTM::LSTM()
  : ComputeOperator("LSTM", ID),
    m_ActivationAlpha(),
    m_ActivationBeta(),
    m_Activations(),
    m_Clip(),
    m_Direction("forward"),
    m_HiddenSize(),
    m_InputForget(0) {
}

LSTM::LSTM(const FloatsAttr& pActivationAlpha,
           const FloatsAttr& pActivationBeta,
           const StringsAttr& pActivations,
           const FloatAttr& pClip,
           const StringAttr& pDirection,
           const IntAttr& pHiddenSize,
           const IntAttr& pInputForget)
  : ComputeOperator("LSTM", ID),
    m_ActivationAlpha(pActivationAlpha),
    m_ActivationBeta(pActivationBeta),
    m_Activations(pActivations),
    m_Clip(pClip),
    m_Direction(pDirection),
    m_HiddenSize(pHiddenSize),
    m_InputForget(pInputForget) {
}

LSTM::LSTM(const LSTM& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_ActivationAlpha(pCopy.getActivationAlpha()),
    m_ActivationBeta(pCopy.getActivationBeta()),
    m_Activations(pCopy.getActivations()),
    m_Clip(pCopy.getClip()),
    m_Direction(pCopy.getDirection()),
    m_HiddenSize(pCopy.getHiddenSize()),
    m_InputForget(pCopy.getInputForget()) {
}

void LSTM::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getActivationAlpha() << ", " << getActivationBeta() << ", " << getActivations() << ", " << getClip() << ", " << getDirection() << ", " << getHiddenSize() << ", " << getInputForget() << ">";
}

bool LSTM::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
