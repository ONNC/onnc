//===- Floor.cpp ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Floor.h>

using namespace onnc;

char Floor::ID = 0;

//===----------------------------------------------------------------------===//
// Floor
//===----------------------------------------------------------------------===//
Floor::Floor()
  : ComputeOperator("Floor", ID) {
}



Floor::Floor(const Floor& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Floor::printAttributes(std::ostream& pOS) const
{
  ;
}

bool Floor::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
