//===- Xor.cpp ------------------------------------------------------------===//
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
  : ComputeOperator("Xor", ID) {
}



Xor::Xor(const Xor& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Xor::printAttributes(std::ostream& pOS) const
{
  ;
}

bool Xor::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
