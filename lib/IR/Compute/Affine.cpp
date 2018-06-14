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

void Affine::print(std::ostream& pOS) const
{
}
