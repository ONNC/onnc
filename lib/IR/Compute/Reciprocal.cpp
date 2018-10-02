//===- Reciprocal.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Reciprocal.h>

using namespace onnc;

char Reciprocal::ID = 0;

//===----------------------------------------------------------------------===//
// Reciprocal
//===----------------------------------------------------------------------===//
Reciprocal::Reciprocal()
  : ComputeOperator("Reciprocal", ID) {
}



Reciprocal::Reciprocal(const Reciprocal& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Reciprocal::printAttributes(std::ostream& pOS) const
{
  ;
}

bool Reciprocal::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
