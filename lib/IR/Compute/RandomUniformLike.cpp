//===- RandomUniformLike.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/RandomUniformLike.h>

using namespace onnc;

char RandomUniformLike::ID = 0;

//===----------------------------------------------------------------------===//
// RandomUniformLike
//===----------------------------------------------------------------------===//
RandomUniformLike::RandomUniformLike()
  : ComputeOperator("RandomUniformLike", ID),
    m_Dtype(),
    m_High(1.0),
    m_Low(0.0),
    m_Seed() {
}

RandomUniformLike::RandomUniformLike(const IntAttr& pDtype,
                                     const FloatAttr& pHigh,
                                     const FloatAttr& pLow,
                                     const FloatAttr& pSeed)
  : ComputeOperator("RandomUniformLike", ID),
    m_Dtype(pDtype),
    m_High(pHigh),
    m_Low(pLow),
    m_Seed(pSeed) {
}

RandomUniformLike::RandomUniformLike(const RandomUniformLike& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Dtype(pCopy.getDtype()),
    m_High(pCopy.getHigh()),
    m_Low(pCopy.getLow()),
    m_Seed(pCopy.getSeed()) {
}

void RandomUniformLike::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getDtype() << ", " << getHigh() << ", " << getLow() << ", " << getSeed() << ">";
}

bool RandomUniformLike::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
