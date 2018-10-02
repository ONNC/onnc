//===- Relu.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Relu.h>

using namespace onnc;

char Relu::ID = 0;

//===----------------------------------------------------------------------===//
// Relu
//===----------------------------------------------------------------------===//
Relu::Relu()
  : ComputeOperator("Relu", ID) {
}



Relu::Relu(const Relu& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Relu::printAttributes(std::ostream& pOS) const
{
  ;
}

bool Relu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
