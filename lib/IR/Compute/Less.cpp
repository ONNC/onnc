//===- Less.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Less.h>

using namespace onnc;

char Less::ID = 0;

//===----------------------------------------------------------------------===//
// Less
//===----------------------------------------------------------------------===//
Less::Less()
  : ComputeOperator("Less", ID) {
}



Less::Less(const Less& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Less::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Less::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
