//===- LRN.cpp ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/LRN.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LRN
//===----------------------------------------------------------------------===//
LRN::LRN(const IntAttr& pSize)
  : ComputeOperator("LRN"),
    m_Alpha(1e-4f),
    m_Beta(0.75f),
    m_Bias(1.0f),
    m_Size(pSize) {
}

LRN::LRN(const FloatAttr& pAlpha,
         const FloatAttr& pBeta,
         const FloatAttr& pBias,
         const IntAttr& pSize)
  : ComputeOperator("LRN"),
    m_Alpha(pAlpha),
    m_Beta(pBeta),
    m_Bias(pBias),
    m_Size(pSize) {
}

void LRN::print(std::ostream& pOS) const
{
  // TODO
}
