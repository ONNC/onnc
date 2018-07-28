//===- Sub.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Sub.h>

using namespace onnc;

char Sub::ID = 0;

//===----------------------------------------------------------------------===//
// Sub
//===----------------------------------------------------------------------===//
Sub::Sub()
  : ComputeOperator("Sub", ID),
    m_Axis(),
    m_Broadcast() {
}

Sub::Sub(const IntAttr& pAxis,
         const IntAttr& pBroadcast)
  : ComputeOperator("Sub", ID),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Sub::print(std::ostream& pOS) const
{
}

bool Sub::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
