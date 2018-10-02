//===- Sum.cpp ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Sum.h>

using namespace onnc;

char Sum::ID = 0;

//===----------------------------------------------------------------------===//
// Sum
//===----------------------------------------------------------------------===//
Sum::Sum()
  : ComputeOperator("Sum", ID) {
}



Sum::Sum(const Sum& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Sum::printAttributes(std::ostream& pOS) const
{
  ;
}

bool Sum::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
