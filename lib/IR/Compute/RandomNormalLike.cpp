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
    m_Mean(),
    m_Scale(),
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

void RandomNormalLike::print(std::ostream& pOS) const
{
}

bool RandomNormalLike::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
