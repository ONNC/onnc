//===- RandomUniform.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/RandomUniform.h>

using namespace onnc;

char RandomUniform::ID = 0;

//===----------------------------------------------------------------------===//
// RandomUniform
//===----------------------------------------------------------------------===//
RandomUniform::RandomUniform()
  : ComputeOperator("RandomUniform", ID),
    m_Dtype(),
    m_High(),
    m_Low(),
    m_Seed(),
    m_Shape() {
}

RandomUniform::RandomUniform(const IntAttr& pDtype,
                             const FloatAttr& pHigh,
                             const FloatAttr& pLow,
                             const FloatAttr& pSeed,
                             const IntsAttr& pShape)
  : ComputeOperator("RandomUniform", ID),
    m_Dtype(pDtype),
    m_High(pHigh),
    m_Low(pLow),
    m_Seed(pSeed),
    m_Shape(pShape) {
}

void RandomUniform::print(std::ostream& pOS) const
{
}

bool RandomUniform::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
