//===- ScaledTanh.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ScaledTanh.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ScaledTanh
//===----------------------------------------------------------------------===//
ScaledTanh::ScaledTanh()
  : ComputeOperator("ScaledTanh"),
    m_Alpha(),
    m_Beta() {
}

ScaledTanh::ScaledTanh(const FloatAttr& pAlpha,
                       const FloatAttr& pBeta)
  : ComputeOperator("ScaledTanh"),
    m_Alpha(pAlpha),
    m_Beta(pBeta) {
}

void ScaledTanh::print(std::ostream& pOS) const
{
}
