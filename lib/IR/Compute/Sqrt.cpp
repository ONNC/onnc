//===- Sqrt.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Sqrt.h>

using namespace onnc;

char Sqrt::ID = 0;

//===----------------------------------------------------------------------===//
// Sqrt
//===----------------------------------------------------------------------===//
Sqrt::Sqrt()
  : ComputeOperator("Sqrt", ID) {
}



Sqrt::Sqrt(const Sqrt& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Sqrt::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Sqrt::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
