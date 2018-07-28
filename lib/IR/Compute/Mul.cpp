//===- Mul.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Mul.h>

using namespace onnc;

char Mul::ID = 0;

//===----------------------------------------------------------------------===//
// Mul
//===----------------------------------------------------------------------===//
Mul::Mul()
  : ComputeOperator("Mul", ID),
    m_Axis(),
    m_Broadcast() {
}

Mul::Mul(const IntAttr& pAxis,
         const IntAttr& pBroadcast)
  : ComputeOperator("Mul", ID),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Mul::print(std::ostream& pOS) const
{
}

bool Mul::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
