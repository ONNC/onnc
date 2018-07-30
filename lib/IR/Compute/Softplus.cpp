//===- Softplus.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Softplus.h>

using namespace onnc;

char Softplus::ID = 0;

//===----------------------------------------------------------------------===//
// Softplus
//===----------------------------------------------------------------------===//
Softplus::Softplus()
  : ComputeOperator("Softplus", ID) {
}



Softplus::Softplus(const Softplus& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Softplus::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Softplus::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
