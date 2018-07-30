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



Softsign::Softsign(const Softsign& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Softsign::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Softsign::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
