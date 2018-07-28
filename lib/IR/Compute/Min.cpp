//===- Min.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Min.h>

using namespace onnc;

char Min::ID = 0;

//===----------------------------------------------------------------------===//
// Min
//===----------------------------------------------------------------------===//
Min::Min()
  : ComputeOperator("Min", ID) {
}



void Min::print(std::ostream& pOS) const
{
}

bool Min::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
