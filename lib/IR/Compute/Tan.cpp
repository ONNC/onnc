//===- Tan.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Tan.h>

using namespace onnc;

char Tan::ID = 0;

//===----------------------------------------------------------------------===//
// Tan
//===----------------------------------------------------------------------===//
Tan::Tan()
  : ComputeOperator("Tan", ID) {
}



void Tan::print(std::ostream& pOS) const
{
}

bool Tan::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
