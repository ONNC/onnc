//===- Max.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Max.h>

using namespace onnc;

char Max::ID = 0;

//===----------------------------------------------------------------------===//
// Max
//===----------------------------------------------------------------------===//
Max::Max()
  : ComputeOperator("Max", ID) {
}



Max::Max(const Max& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Max::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Max::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
