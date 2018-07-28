//===- Exp.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Exp.h>

using namespace onnc;

char Exp::ID = 0;

//===----------------------------------------------------------------------===//
// Exp
//===----------------------------------------------------------------------===//
Exp::Exp()
  : ComputeOperator("Exp", ID) {
}



void Exp::print(std::ostream& pOS) const
{
}

bool Exp::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
