//===- HardSigmoid.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/HardSigmoid.h>

using namespace onnc;

char HardSigmoid::ID = 0;

//===----------------------------------------------------------------------===//
// HardSigmoid
//===----------------------------------------------------------------------===//
HardSigmoid::HardSigmoid()
  : ComputeOperator("HardSigmoid", ID),
    m_Alpha(0.20000000298),
    m_Beta(0.5) {
}

HardSigmoid::HardSigmoid(const FloatAttr& pAlpha,
                         const FloatAttr& pBeta)
  : ComputeOperator("HardSigmoid", ID),
    m_Alpha(pAlpha),
    m_Beta(pBeta) {
}

HardSigmoid::HardSigmoid(const HardSigmoid& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Alpha(pCopy.getAlpha()),
    m_Beta(pCopy.getBeta()) {
}

void HardSigmoid::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAlpha() << ", " << getBeta() << ">";
}

bool HardSigmoid::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
