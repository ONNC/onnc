//===- Sub.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Sub.h>

using namespace onnc;

char Sub::ID = 0;

//===----------------------------------------------------------------------===//
// Sub
//===----------------------------------------------------------------------===//
Sub::Sub()
  : ComputeOperator("Sub", ID) {
}



Sub::Sub(const Sub& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Sub::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Sub::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
