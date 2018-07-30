//===- Equal.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Equal.h>

using namespace onnc;

char Equal::ID = 0;

//===----------------------------------------------------------------------===//
// Equal
//===----------------------------------------------------------------------===//
Equal::Equal()
  : ComputeOperator("Equal", ID) {
}



Equal::Equal(const Equal& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Equal::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Equal::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
