//===- Multinomial.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Multinomial.h>

using namespace onnc;

char Multinomial::ID = 0;

//===----------------------------------------------------------------------===//
// Multinomial
//===----------------------------------------------------------------------===//
Multinomial::Multinomial()
  : ComputeOperator("Multinomial", ID),
    m_Dtype(6),
    m_SampleSize(1),
    m_Seed() {
}

Multinomial::Multinomial(const IntAttr& pDtype,
                         const IntAttr& pSampleSize,
                         const FloatAttr& pSeed)
  : ComputeOperator("Multinomial", ID),
    m_Dtype(pDtype),
    m_SampleSize(pSampleSize),
    m_Seed(pSeed) {
}

Multinomial::Multinomial(const Multinomial& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Dtype(pCopy.getDtype()),
    m_SampleSize(pCopy.getSampleSize()),
    m_Seed(pCopy.getSeed()) {
}

void Multinomial::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getDtype() << ", " << getSampleSize() << ", " << getSeed() << ">";
}

bool Multinomial::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
