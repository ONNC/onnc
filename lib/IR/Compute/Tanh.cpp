//===- Tanh.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Tanh.h>

using namespace onnc;

char Tanh::ID = 0;

//===----------------------------------------------------------------------===//
// Tanh
//===----------------------------------------------------------------------===//
Tanh::Tanh()
  : ComputeOperator("Tanh", ID) {
}



Tanh::Tanh(const Tanh& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Tanh::printAttributes(std::ostream& pOS) const
{
  ;
}

bool Tanh::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
