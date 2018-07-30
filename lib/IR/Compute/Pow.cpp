//===- Pow.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Pow.h>

using namespace onnc;

char Pow::ID = 0;

//===----------------------------------------------------------------------===//
// Pow
//===----------------------------------------------------------------------===//
Pow::Pow()
  : ComputeOperator("Pow", ID) {
}



Pow::Pow(const Pow& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Pow::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Pow::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
