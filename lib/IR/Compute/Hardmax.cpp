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
    m_Axis() {
}

Hardmax::Hardmax(const IntAttr& pAxis)
  : ComputeOperator("Hardmax", ID),
    m_Axis(pAxis) {
}

void Hardmax::print(std::ostream& pOS) const
{
}

bool Hardmax::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
