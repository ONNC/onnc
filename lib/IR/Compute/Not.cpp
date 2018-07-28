//===- Not.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Not.h>

using namespace onnc;

char Not::ID = 0;

//===----------------------------------------------------------------------===//
// Not
//===----------------------------------------------------------------------===//
Not::Not()
  : ComputeOperator("Not", ID) {
}



void Not::print(std::ostream& pOS) const
{
}

bool Not::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
