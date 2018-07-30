//===- LRN.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/LRN.h>

using namespace onnc;

char LRN::ID = 0;

//===----------------------------------------------------------------------===//
// LRN
//===----------------------------------------------------------------------===//
LRN::LRN(const IntAttr& pSize)
  : ComputeOperator("LRN", ID),
    m_Alpha(9.99999974738e-05),
    m_Beta(0.75),
    m_Bias(1.0),
    m_Size(pSize) {
}

LRN::LRN(const FloatAttr& pAlpha,
         const FloatAttr& pBeta,
         const FloatAttr& pBias,
         const IntAttr& pSize)
  : ComputeOperator("LRN", ID),
    m_Alpha(pAlpha),
    m_Beta(pBeta),
    m_Bias(pBias),
    m_Size(pSize) {
}

LRN::LRN(const LRN& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Alpha(pCopy.getAlpha()),
    m_Beta(pCopy.getBeta()),
    m_Bias(pCopy.getBias()),
    m_Size(pCopy.getSize()) {
}

void LRN::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAlpha() << ", " << getBeta() << ", " << getBias() << ", " << getSize() << ">";
}

bool LRN::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
