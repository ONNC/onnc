//===- Or.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Or.h>

using namespace onnc;

char Or::ID = 0;

//===----------------------------------------------------------------------===//
// Or
//===----------------------------------------------------------------------===//
Or::Or()
  : ComputeOperator("Or", ID) {
}



Or::Or(const Or& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Or::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Or::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
