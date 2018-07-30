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
RandomUniform::RandomUniform(const IntsAttr& pShape)
  : ComputeOperator("RandomUniform", ID),
    m_Dtype(1),
    m_High(1.0),
    m_Low(0.0),
    m_Seed(),
    m_Shape(pShape) {
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

RandomUniform::RandomUniform(const RandomUniform& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Dtype(pCopy.getDtype()),
    m_High(pCopy.getHigh()),
    m_Low(pCopy.getLow()),
    m_Seed(pCopy.getSeed()),
    m_Shape(pCopy.getShape()) {
}

void RandomUniform::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getDtype() << ", " << getHigh() << ", " << getLow() << ", " << getSeed() << ", " << getShape() << ">";
}

bool RandomUniform::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
