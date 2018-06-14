//===- LRN.cpp ------------------------------------------------===//
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
LRN::LRN()
  : ComputeOperator("LRN"),
    m_Alpha(),
    m_Beta(),
    m_Bias(),
    m_Size() {
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
}
