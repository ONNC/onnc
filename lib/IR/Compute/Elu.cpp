//===- Elu.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Elu.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Elu
//===----------------------------------------------------------------------===//
Elu::Elu()
  : ComputeOperator("Elu"),
    m_Alpha() {
}

Elu::Elu(const FloatAttr& pAlpha)
  : ComputeOperator("Elu"),
    m_Alpha(pAlpha) {
}

void Elu::print(std::ostream& pOS) const
{
}
