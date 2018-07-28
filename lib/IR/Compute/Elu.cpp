//===- Elu.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Elu.h>

using namespace onnc;

char Elu::ID = 0;

//===----------------------------------------------------------------------===//
// Elu
//===----------------------------------------------------------------------===//
Elu::Elu()
  : ComputeOperator("Elu", ID),
    m_Alpha() {
}

Elu::Elu(const FloatAttr& pAlpha)
  : ComputeOperator("Elu", ID),
    m_Alpha(pAlpha) {
}

void Elu::print(std::ostream& pOS) const
{
}

bool Elu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
