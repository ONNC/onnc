//===- Or.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Or.h>

using namespace onnc;

char Or::ID = 0;

//===----------------------------------------------------------------------===//
// Or
//===----------------------------------------------------------------------===//
Or::Or()
  : ComputeOperator("Or", ID),
    m_Axis(),
    m_Broadcast() {
}

Or::Or(const IntAttr& pAxis,
       const IntAttr& pBroadcast)
  : ComputeOperator("Or", ID),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Or::print(std::ostream& pOS) const
{
}

bool Or::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
