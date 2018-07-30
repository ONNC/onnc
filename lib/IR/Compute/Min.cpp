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



Min::Min(const Min& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Min::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Min::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
