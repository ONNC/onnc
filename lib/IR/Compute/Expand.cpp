//===- Expand.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Expand.h>

using namespace onnc;

char Expand::ID = 0;

//===----------------------------------------------------------------------===//
// Expand
//===----------------------------------------------------------------------===//
Expand::Expand()
  : ComputeOperator("Expand", ID) {
}



Expand::Expand(const Expand& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Expand::printAttributes(std::ostream& pOS) const
{
  ;
}

bool Expand::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
