//===- Ceil.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Ceil.h>

using namespace onnc;

char Ceil::ID = 0;

//===----------------------------------------------------------------------===//
// Ceil
//===----------------------------------------------------------------------===//
Ceil::Ceil()
  : ComputeOperator("Ceil", ID) {
}



Ceil::Ceil(const Ceil& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Ceil::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Ceil::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
