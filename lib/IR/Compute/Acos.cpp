//===- Acos.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Acos.h>

using namespace onnc;

char Acos::ID = 0;

//===----------------------------------------------------------------------===//
// Acos
//===----------------------------------------------------------------------===//
Acos::Acos()
  : ComputeOperator("Acos", ID) {
}

Acos::Acos(const Acos& pCopy)
  : ComputeOperator(pCopy) {
}

void Acos::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Acos::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
