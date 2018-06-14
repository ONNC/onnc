//===- HardSigmoid.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/HardSigmoid.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// HardSigmoid
//===----------------------------------------------------------------------===//
HardSigmoid::HardSigmoid()
  : ComputeOperator("HardSigmoid"),
    m_Alpha(),
    m_Beta() {
}

HardSigmoid::HardSigmoid(const FloatAttr& pAlpha,
                         const FloatAttr& pBeta)
  : ComputeOperator("HardSigmoid"),
    m_Alpha(pAlpha),
    m_Beta(pBeta) {
}

void HardSigmoid::print(std::ostream& pOS) const
{
}
