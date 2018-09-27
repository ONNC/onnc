//===- Cos.cpp ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Cos.h>

using namespace onnc;

char Cos::ID = 0;

//===----------------------------------------------------------------------===//
// Cos
//===----------------------------------------------------------------------===//
Cos::Cos()
  : ComputeOperator("Cos", ID) {
}



Cos::Cos(const Cos& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Cos::printAttributes(std::ostream& pOS) const
{
  ;
}

bool Cos::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
