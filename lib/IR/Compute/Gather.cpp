//===- Gather.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Gather.h>

using namespace onnc;

char Gather::ID = 0;

//===----------------------------------------------------------------------===//
// Gather
//===----------------------------------------------------------------------===//
Gather::Gather()
  : ComputeOperator("Gather", ID),
    m_Axis() {
}

Gather::Gather(const IntAttr& pAxis)
  : ComputeOperator("Gather", ID),
    m_Axis(pAxis) {
}

void Gather::print(std::ostream& pOS) const
{
}

bool Gather::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
