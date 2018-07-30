//===- Hardmax.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Hardmax.h>

using namespace onnc;

char Hardmax::ID = 0;

//===----------------------------------------------------------------------===//
// Hardmax
//===----------------------------------------------------------------------===//
Hardmax::Hardmax()
  : ComputeOperator("Hardmax", ID),
    m_Axis(1) {
}

Hardmax::Hardmax(const IntAttr& pAxis)
  : ComputeOperator("Hardmax", ID),
    m_Axis(pAxis) {
}

Hardmax::Hardmax(const Hardmax& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axis(pCopy.getAxis()) {
}

void Hardmax::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxis() << ">";
}

bool Hardmax::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
