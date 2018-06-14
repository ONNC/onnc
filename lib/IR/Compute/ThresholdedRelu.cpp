//===- ThresholdedRelu.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ThresholdedRelu.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ThresholdedRelu
//===----------------------------------------------------------------------===//
ThresholdedRelu::ThresholdedRelu()
  : ComputeOperator("ThresholdedRelu"),
    m_Alpha() {
}

ThresholdedRelu::ThresholdedRelu(const FloatAttr& pAlpha)
  : ComputeOperator("ThresholdedRelu"),
    m_Alpha(pAlpha) {
}

void ThresholdedRelu::print(std::ostream& pOS) const
{
}
