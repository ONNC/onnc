//===- Affine.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Affine.h>

using namespace onnc;

char Affine::ID = 0;

//===----------------------------------------------------------------------===//
// Affine
//===----------------------------------------------------------------------===//
Affine::Affine()
  : ComputeOperator("Affine", ID),
    m_Alpha(1.0),
    m_Beta(0.0) {
}

Affine::Affine(const FloatAttr& pAlpha,
               const FloatAttr& pBeta)
  : ComputeOperator("Affine", ID),
    m_Alpha(pAlpha),
    m_Beta(pBeta) {
}

Affine::Affine(const Affine& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Alpha(pCopy.getAlpha()),
    m_Beta(pCopy.getBeta()) {
}

void Affine::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAlpha() << ", " << getBeta() << ">";
}

bool Affine::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
