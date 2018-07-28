//===- And.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/And.h>

using namespace onnc;

char And::ID = 0;

//===----------------------------------------------------------------------===//
// And
//===----------------------------------------------------------------------===//
And::And()
  : ComputeOperator("And", ID),
    m_Axis(),
    m_Broadcast() {
}

And::And(const IntAttr& pAxis,
         const IntAttr& pBroadcast)
  : ComputeOperator("And", ID),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void And::print(std::ostream& pOS) const
{
}

bool And::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
