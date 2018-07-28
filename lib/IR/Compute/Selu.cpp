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
    m_Alpha(),
    m_Gamma() {
}

Selu::Selu(const FloatAttr& pAlpha,
           const FloatAttr& pGamma)
  : ComputeOperator("Selu", ID),
    m_Alpha(pAlpha),
    m_Gamma(pGamma) {
}

void Selu::print(std::ostream& pOS) const
{
}

bool Selu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
