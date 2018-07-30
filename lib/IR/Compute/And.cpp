//===- And.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/And.h>

using namespace onnc;

char And::ID = 0;

//===----------------------------------------------------------------------===//
// And
//===----------------------------------------------------------------------===//
And::And()
  : ComputeOperator("And", ID) {
}



And::And(const And& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void And::print(std::ostream& pOS) const
{
  pOS << name();
}

bool And::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
