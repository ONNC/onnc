//===- Dropout.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Dropout.h>

using namespace onnc;

char Dropout::ID = 0;

//===----------------------------------------------------------------------===//
// Dropout
//===----------------------------------------------------------------------===//
Dropout::Dropout()
  : ComputeOperator("Dropout", ID),
    m_IsTest(),
    m_Ratio() {
}

Dropout::Dropout(const IntAttr& pIsTest,
                 const FloatAttr& pRatio)
  : ComputeOperator("Dropout", ID),
    m_IsTest(pIsTest),
    m_Ratio(pRatio) {
}

void Dropout::print(std::ostream& pOS) const
{
}

bool Dropout::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
