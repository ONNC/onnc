//===- Softsign.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Softsign.h>

using namespace onnc;

char Softsign::ID = 0;

//===----------------------------------------------------------------------===//
// Softsign
//===----------------------------------------------------------------------===//
Softsign::Softsign()
  : ComputeOperator("Softsign", ID) {
}



void Softsign::print(std::ostream& pOS) const
{
}

bool Softsign::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
