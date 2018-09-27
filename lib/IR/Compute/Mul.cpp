//===- Mul.cpp ------------------------------------------------------------===//
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
  : ComputeOperator("Mul", ID) {
}



Mul::Mul(const Mul& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Mul::printAttributes(std::ostream& pOS) const
{
  ;
}

bool Mul::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
