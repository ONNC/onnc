//===- Reshape.cpp --------------------------------------------------------===//
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



Reshape::Reshape(const Reshape& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void Reshape::printAttributes(std::ostream& pOS) const
{
  ;
}

bool Reshape::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
