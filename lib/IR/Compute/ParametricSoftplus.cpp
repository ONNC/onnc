//===- ParametricSoftplus.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ParametricSoftplus.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ParametricSoftplus
//===----------------------------------------------------------------------===//
ParametricSoftplus::ParametricSoftplus()
  : ComputeOperator("ParametricSoftplus"),
    m_Alpha(),
    m_Beta() {
}

ParametricSoftplus::ParametricSoftplus(const FloatAttr& pAlpha,
                                       const FloatAttr& pBeta)
  : ComputeOperator("ParametricSoftplus"),
    m_Alpha(pAlpha),
    m_Beta(pBeta) {
}

void ParametricSoftplus::print(std::ostream& pOS) const
{
}
