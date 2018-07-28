//===- Cos.cpp ------------------------------------------------===//
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



void Cos::print(std::ostream& pOS) const
{
}

bool Cos::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
