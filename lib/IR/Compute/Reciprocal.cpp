//===- Reciprocal.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Reciprocal.h>

using namespace onnc;

char Reciprocal::ID = 0;

//===----------------------------------------------------------------------===//
// Reciprocal
//===----------------------------------------------------------------------===//
Reciprocal::Reciprocal()
  : ComputeOperator("Reciprocal", ID) {
}



void Reciprocal::print(std::ostream& pOS) const
{
}

bool Reciprocal::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
