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
    m_Ratio(0.5) {
}

Dropout::Dropout(const FloatAttr& pRatio)
  : ComputeOperator("Dropout", ID),
    m_Ratio(pRatio) {
}

Dropout::Dropout(const Dropout& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Ratio(pCopy.getRatio()) {
}

void Dropout::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getRatio() << ">";
}

bool Dropout::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
