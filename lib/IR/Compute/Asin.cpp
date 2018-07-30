//===- Asin.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Asin.h>

using namespace onnc;

char Asin::ID = 0;

//===----------------------------------------------------------------------===//
// Asin
//===----------------------------------------------------------------------===//
Asin::Asin()
  : ComputeOperator("Asin", ID) {
}



Asin::Asin(const Asin& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Asin::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Asin::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
