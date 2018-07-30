//===- MatMul.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/MatMul.h>

using namespace onnc;

char MatMul::ID = 0;

//===----------------------------------------------------------------------===//
// MatMul
//===----------------------------------------------------------------------===//
MatMul::MatMul()
  : ComputeOperator("MatMul", ID) {
}



MatMul::MatMul(const MatMul& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */ {
}

void MatMul::print(std::ostream& pOS) const
{
  pOS << name();
}

bool MatMul::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
