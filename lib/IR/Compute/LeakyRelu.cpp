//===- LeakyRelu.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/LeakyRelu.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LeakyRelu
//===----------------------------------------------------------------------===//
LeakyRelu::LeakyRelu()
  : ComputeOperator("LeakyRelu"),
    m_Alpha() {
}

LeakyRelu::LeakyRelu(const FloatAttr& pAlpha)
  : ComputeOperator("LeakyRelu"),
    m_Alpha(pAlpha) {
}

void LeakyRelu::print(std::ostream& pOS) const
{
}
