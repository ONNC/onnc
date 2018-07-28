//===- Atan.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Atan.h>

using namespace onnc;

char Atan::ID = 0;

//===----------------------------------------------------------------------===//
// Atan
//===----------------------------------------------------------------------===//
Atan::Atan()
  : ComputeOperator("Atan", ID) {
}



void Atan::print(std::ostream& pOS) const
{
}

bool Atan::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
