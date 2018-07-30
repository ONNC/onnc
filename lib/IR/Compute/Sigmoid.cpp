//===- Sigmoid.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Sigmoid.h>

using namespace onnc;

char Sigmoid::ID = 0;

//===----------------------------------------------------------------------===//
// Sigmoid
//===----------------------------------------------------------------------===//
Sigmoid::Sigmoid()
  : ComputeOperator("Sigmoid", ID) {
}



Sigmoid::Sigmoid(const Sigmoid& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Sigmoid::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Sigmoid::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
