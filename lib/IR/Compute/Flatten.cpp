//===- Flatten.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Flatten.h>

using namespace onnc;

char Flatten::ID = 0;

//===----------------------------------------------------------------------===//
// Flatten
//===----------------------------------------------------------------------===//
Flatten::Flatten()
  : ComputeOperator("Flatten", ID),
    m_Axis() {
}

Flatten::Flatten(const IntAttr& pAxis)
  : ComputeOperator("Flatten", ID),
    m_Axis(pAxis) {
}

void Flatten::print(std::ostream& pOS) const
{
}

bool Flatten::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
