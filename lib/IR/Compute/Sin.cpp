//===- Sin.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Sin.h>

using namespace onnc;

char Sin::ID = 0;

//===----------------------------------------------------------------------===//
// Sin
//===----------------------------------------------------------------------===//
Sin::Sin()
  : ComputeOperator("Sin", ID) {
}



Sin::Sin(const Sin& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Sin::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Sin::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
