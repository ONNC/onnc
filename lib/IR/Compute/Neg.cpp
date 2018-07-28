//===- Neg.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Neg.h>

using namespace onnc;

char Neg::ID = 0;

//===----------------------------------------------------------------------===//
// Neg
//===----------------------------------------------------------------------===//
Neg::Neg()
  : ComputeOperator("Neg", ID) {
}



void Neg::print(std::ostream& pOS) const
{
}

bool Neg::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
