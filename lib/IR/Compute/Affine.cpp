//===- Affine.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Affine.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Affine
//===----------------------------------------------------------------------===//
Affine::Affine()
  : ComputeOperator("Affine"),
    m_Alpha(),
    m_Beta() {
}

Affine::Affine(const FloatAttr& pAlpha,
               const FloatAttr& pBeta)
  : ComputeOperator("Affine"),
    m_Alpha(pAlpha),
    m_Beta(pBeta) {
}

Affine::Affine(const Affine& pCopy)
  : ComputeOperator(pCopy),
    m_Alpha(pCopy.getAlpha()),
    m_Beta(pCopy.getBeta()) {
}
void Affine::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAlpha() << ", " << getBeta() << ">";
}
