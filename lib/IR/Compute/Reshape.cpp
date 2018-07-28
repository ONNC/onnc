//===- Reshape.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Reshape.h>

using namespace onnc;

char Reshape::ID = 0;

//===----------------------------------------------------------------------===//
// Reshape
//===----------------------------------------------------------------------===//
Reshape::Reshape()
  : ComputeOperator("Reshape", ID) {
}

void Reshape::print(std::ostream& pOS) const
{
}

bool Reshape::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
