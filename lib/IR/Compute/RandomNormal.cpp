//===- RandomNormal.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/RandomNormal.h>

using namespace onnc;

char RandomNormal::ID = 0;

//===----------------------------------------------------------------------===//
// RandomNormal
//===----------------------------------------------------------------------===//
RandomNormal::RandomNormal()
  : ComputeOperator("RandomNormal", ID),
    m_Dtype(),
    m_Mean(),
    m_Scale(),
    m_Seed(),
    m_Shape() {
}

RandomNormal::RandomNormal(const IntAttr& pDtype,
                           const FloatAttr& pMean,
                           const FloatAttr& pScale,
                           const FloatAttr& pSeed,
                           const IntsAttr& pShape)
  : ComputeOperator("RandomNormal", ID),
    m_Dtype(pDtype),
    m_Mean(pMean),
    m_Scale(pScale),
    m_Seed(pSeed),
    m_Shape(pShape) {
}

void RandomNormal::print(std::ostream& pOS) const
{
}

bool RandomNormal::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
