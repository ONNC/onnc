//===- Less.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Less.h>

using namespace onnc;

char Less::ID = 0;

//===----------------------------------------------------------------------===//
// Less
//===----------------------------------------------------------------------===//
Less::Less()
  : ComputeOperator("Less", ID),
    m_Axis(),
    m_Broadcast() {
}

Less::Less(const IntAttr& pAxis,
           const IntAttr& pBroadcast)
  : ComputeOperator("Less", ID),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Less::print(std::ostream& pOS) const
{
}

bool Less::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
