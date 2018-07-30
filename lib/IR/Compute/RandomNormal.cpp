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
RandomNormal::RandomNormal(const IntsAttr& pShape)
  : ComputeOperator("RandomNormal", ID),
    m_Dtype(1),
    m_Mean(0.0),
    m_Scale(1.0),
    m_Seed(),
    m_Shape(pShape) {
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

RandomNormal::RandomNormal(const RandomNormal& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Dtype(pCopy.getDtype()),
    m_Mean(pCopy.getMean()),
    m_Scale(pCopy.getScale()),
    m_Seed(pCopy.getSeed()),
    m_Shape(pCopy.getShape()) {
}

void RandomNormal::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getDtype() << ", " << getMean() << ", " << getScale() << ", " << getSeed() << ", " << getShape() << ">";
}

bool RandomNormal::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
