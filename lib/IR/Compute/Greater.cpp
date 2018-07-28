//===- Greater.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Greater.h>

using namespace onnc;

char Greater::ID = 0;

//===----------------------------------------------------------------------===//
// Greater
//===----------------------------------------------------------------------===//
Greater::Greater()
  : ComputeOperator("Greater", ID),
    m_Axis(),
    m_Broadcast() {
}

Greater::Greater(const IntAttr& pAxis,
                 const IntAttr& pBroadcast)
  : ComputeOperator("Greater", ID),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Greater::print(std::ostream& pOS) const
{
}

bool Greater::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
