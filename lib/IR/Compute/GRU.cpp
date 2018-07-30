//===- GRU.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/GRU.h>

using namespace onnc;

char GRU::ID = 0;

//===----------------------------------------------------------------------===//
// GRU
//===----------------------------------------------------------------------===//
GRU::GRU()
  : ComputeOperator("GRU", ID),
    m_ActivationAlpha(),
    m_ActivationBeta(),
    m_Activations(),
    m_Clip(),
    m_Direction("forward"),
    m_HiddenSize(),
    m_LinearBeforeReset(0) {
}

GRU::GRU(const FloatsAttr& pActivationAlpha,
         const FloatsAttr& pActivationBeta,
         const StringsAttr& pActivations,
         const FloatAttr& pClip,
         const StringAttr& pDirection,
         const IntAttr& pHiddenSize,
         const IntAttr& pLinearBeforeReset)
  : ComputeOperator("GRU", ID),
    m_ActivationAlpha(pActivationAlpha),
    m_ActivationBeta(pActivationBeta),
    m_Activations(pActivations),
    m_Clip(pClip),
    m_Direction(pDirection),
    m_HiddenSize(pHiddenSize),
    m_LinearBeforeReset(pLinearBeforeReset) {
}

GRU::GRU(const GRU& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_ActivationAlpha(pCopy.getActivationAlpha()),
    m_ActivationBeta(pCopy.getActivationBeta()),
    m_Activations(pCopy.getActivations()),
    m_Clip(pCopy.getClip()),
    m_Direction(pCopy.getDirection()),
    m_HiddenSize(pCopy.getHiddenSize()),
    m_LinearBeforeReset(pCopy.getLinearBeforeReset()) {
}

void GRU::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getActivationAlpha() << ", " << getActivationBeta() << ", " << getActivations() << ", " << getClip() << ", " << getDirection() << ", " << getHiddenSize() << ", " << getLinearBeforeReset() << ">";
}

bool GRU::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
