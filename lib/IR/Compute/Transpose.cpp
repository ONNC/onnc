//===- Transpose.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Transpose.h>

using namespace onnc;

char Transpose::ID = 0;

//===----------------------------------------------------------------------===//
// Transpose
//===----------------------------------------------------------------------===//
Transpose::Transpose()
  : ComputeOperator("Transpose", ID),
    m_Perm() {
}

Transpose::Transpose(const IntsAttr& pPerm)
  : ComputeOperator("Transpose", ID),
    m_Perm(pPerm) {
}

void Transpose::print(std::ostream& pOS) const
{
}

bool Transpose::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
