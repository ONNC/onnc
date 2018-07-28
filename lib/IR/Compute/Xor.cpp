//===- Xor.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Xor.h>

using namespace onnc;

char Xor::ID = 0;

//===----------------------------------------------------------------------===//
// Xor
//===----------------------------------------------------------------------===//
Xor::Xor()
  : ComputeOperator("Xor", ID),
    m_Axis(),
    m_Broadcast() {
}

Xor::Xor(const IntAttr& pAxis,
         const IntAttr& pBroadcast)
  : ComputeOperator("Xor", ID),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Xor::print(std::ostream& pOS) const
{
}

bool Xor::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
