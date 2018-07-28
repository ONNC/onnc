//===- Floor.cpp ------------------------------------------------===//
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



void Floor::print(std::ostream& pOS) const
{
}

bool Floor::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
