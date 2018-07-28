//===- Abs.cpp ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Abs.h>

using namespace onnc;

char Abs::ID = 0;

//===----------------------------------------------------------------------===//
// Abs
//===----------------------------------------------------------------------===//
Abs::Abs()
  : ComputeOperator("Abs", ID) {
}

Abs::Abs(const Abs& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Abs::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Abs::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
