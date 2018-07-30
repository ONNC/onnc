//===- Div.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Div.h>

using namespace onnc;

char Div::ID = 0;

//===----------------------------------------------------------------------===//
// Div
//===----------------------------------------------------------------------===//
Div::Div()
  : ComputeOperator("Div", ID) {
}



Div::Div(const Div& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Div::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Div::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
