//===- ATen.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ATen.h>

using namespace onnc;

char ATen::ID = 0;

//===----------------------------------------------------------------------===//
// ATen
//===----------------------------------------------------------------------===//
ATen::ATen()
  : ComputeOperator("ATen", ID) {
}



ATen::ATen(const ATen& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void ATen::print(std::ostream& pOS) const
{
  pOS << name();
}

bool ATen::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
