//===- Neg.cpp ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Neg.h>

using namespace onnc;

char Neg::ID = 0;

//===----------------------------------------------------------------------===//
// Neg
//===----------------------------------------------------------------------===//
Neg::Neg()
  : ComputeOperator("Neg", ID) {
}



Neg::Neg(const Neg& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Neg::printAttributes(std::ostream& pOS) const
{
  ;
}

bool Neg::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
