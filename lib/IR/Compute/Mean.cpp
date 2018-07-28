//===- Mean.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Mean.h>

using namespace onnc;

char Mean::ID = 0;

//===----------------------------------------------------------------------===//
// Mean
//===----------------------------------------------------------------------===//
Mean::Mean()
  : ComputeOperator("Mean", ID) {
}



void Mean::print(std::ostream& pOS) const
{
}

bool Mean::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
