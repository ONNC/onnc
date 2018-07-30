//===- Tan.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Tan.h>

using namespace onnc;

char Tan::ID = 0;

//===----------------------------------------------------------------------===//
// Tan
//===----------------------------------------------------------------------===//
Tan::Tan()
  : ComputeOperator("Tan", ID) {
}



Tan::Tan(const Tan& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Tan::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Tan::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
