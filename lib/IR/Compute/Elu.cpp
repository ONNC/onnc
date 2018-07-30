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
    m_Alpha(1.0) {
}

Elu::Elu(const FloatAttr& pAlpha)
  : ComputeOperator("Elu", ID),
    m_Alpha(pAlpha) {
}

Elu::Elu(const Elu& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Alpha(pCopy.getAlpha()) {
}

void Elu::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAlpha() << ">";
}

bool Elu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
