//===- RandomNormalLike.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/RandomNormalLike.h>

using namespace onnc;

char RandomNormalLike::ID = 0;

//===----------------------------------------------------------------------===//
// RandomNormalLike
//===----------------------------------------------------------------------===//
RandomNormalLike::RandomNormalLike()
  : ComputeOperator("RandomNormalLike", ID),
    m_Dtype(),
    m_Mean(0.0),
    m_Scale(1.0),
    m_Seed() {
}

RandomNormalLike::RandomNormalLike(const IntAttr& pDtype,
                                   const FloatAttr& pMean,
                                   const FloatAttr& pScale,
                                   const FloatAttr& pSeed)
  : ComputeOperator("RandomNormalLike", ID),
    m_Dtype(pDtype),
    m_Mean(pMean),
    m_Scale(pScale),
    m_Seed(pSeed) {
}

RandomNormalLike::RandomNormalLike(const RandomNormalLike& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Dtype(pCopy.getDtype()),
    m_Mean(pCopy.getMean()),
    m_Scale(pCopy.getScale()),
    m_Seed(pCopy.getSeed()) {
}

void RandomNormalLike::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getDtype() << ", " << getMean() << ", " << getScale() << ", " << getSeed() << ">";
}

bool RandomNormalLike::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
