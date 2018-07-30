//===- Selu.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Selu.h>

using namespace onnc;

char Selu::ID = 0;

//===----------------------------------------------------------------------===//
// Selu
//===----------------------------------------------------------------------===//
Selu::Selu()
  : ComputeOperator("Selu", ID),
    m_Alpha(1.67326319218),
    m_Gamma(1.05070102215) {
}

Selu::Selu(const FloatAttr& pAlpha,
           const FloatAttr& pGamma)
  : ComputeOperator("Selu", ID),
    m_Alpha(pAlpha),
    m_Gamma(pGamma) {
}

Selu::Selu(const Selu& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Alpha(pCopy.getAlpha()),
    m_Gamma(pCopy.getGamma()) {
}

void Selu::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAlpha() << ", " << getGamma() << ">";
}

bool Selu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
