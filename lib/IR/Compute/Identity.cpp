//===- Identity.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Identity.h>

using namespace onnc;

char Identity::ID = 0;

//===----------------------------------------------------------------------===//
// Identity
//===----------------------------------------------------------------------===//
Identity::Identity()
  : ComputeOperator("Identity", ID) {
}



Identity::Identity(const Identity& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Identity::printAttributes(std::ostream& pOS) const
{
  ;
}

bool Identity::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
