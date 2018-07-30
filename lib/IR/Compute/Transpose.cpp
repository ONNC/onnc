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

Transpose::Transpose(const Transpose& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Perm(pCopy.getPerm()) {
}

void Transpose::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getPerm() << ">";
}

bool Transpose::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
