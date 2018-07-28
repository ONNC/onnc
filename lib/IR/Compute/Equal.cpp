//===- Equal.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Equal.h>

using namespace onnc;

char Equal::ID = 0;

//===----------------------------------------------------------------------===//
// Equal
//===----------------------------------------------------------------------===//
Equal::Equal()
  : ComputeOperator("Equal", ID),
    m_Axis(),
    m_Broadcast() {
}

Equal::Equal(const IntAttr& pAxis,
             const IntAttr& pBroadcast)
  : ComputeOperator("Equal", ID),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Equal::print(std::ostream& pOS) const
{
}

bool Equal::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
