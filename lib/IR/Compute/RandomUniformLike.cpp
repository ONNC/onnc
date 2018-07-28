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
    m_High(),
    m_Low(),
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

void RandomUniformLike::print(std::ostream& pOS) const
{
}

bool RandomUniformLike::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
