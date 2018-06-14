//===- Selu.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Selu.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Selu
//===----------------------------------------------------------------------===//
Selu::Selu()
  : ComputeOperator("Selu"),
    m_Alpha(),
    m_Gamma() {
}

Selu::Selu(const FloatAttr& pAlpha,
           const FloatAttr& pGamma)
  : ComputeOperator("Selu"),
    m_Alpha(pAlpha),
    m_Gamma(pGamma) {
}

void Selu::print(std::ostream& pOS) const
{
}
