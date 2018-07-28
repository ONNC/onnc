//===- Relu.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Relu.h>

using namespace onnc;

char Relu::ID = 0;

//===----------------------------------------------------------------------===//
// Relu
//===----------------------------------------------------------------------===//
Relu::Relu()
  : ComputeOperator("Relu", ID) {
}

void Relu::print(std::ostream& pOS) const
{
  pOS << name();
}

bool Relu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
