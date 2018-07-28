//===- Pow.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Pow.h>

using namespace onnc;

char Pow::ID = 0;

//===----------------------------------------------------------------------===//
// Pow
//===----------------------------------------------------------------------===//
Pow::Pow()
  : ComputeOperator("Pow", ID),
    m_Axis(),
    m_Broadcast() {
}

Pow::Pow(const IntAttr& pAxis,
         const IntAttr& pBroadcast)
  : ComputeOperator("Pow", ID),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Pow::print(std::ostream& pOS) const
{
}

bool Pow::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
