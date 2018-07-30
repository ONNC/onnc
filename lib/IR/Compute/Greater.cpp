//===- Greater.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Greater.h>

using namespace onnc;

char Greater::ID = 0;

//===----------------------------------------------------------------------===//
// Greater
//===----------------------------------------------------------------------===//
Greater::Greater()
  : ComputeOperator("Greater", ID) {
}



Greater::Greater(const Greater& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Greater::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Greater::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
