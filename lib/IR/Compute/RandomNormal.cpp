//===- RandomNormal.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/RandomNormal.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// RandomNormal
//===----------------------------------------------------------------------===//
RandomNormal::RandomNormal()
  : ComputeOperator("RandomNormal"),
    m_Dtype(),
    m_Mean(),
    m_Scale(),
    m_Seed(),
    m_Shape() {
}

RandomNormal::RandomNormal(const IntAttr& pDtype,
                           const FloatAttr& pMean,
                           const FloatAttr& pScale,
                           const FloatAttr& pSeed,
                           const IntsAttr& pShape)
  : ComputeOperator("RandomNormal"),
    m_Dtype(pDtype),
    m_Mean(pMean),
    m_Scale(pScale),
    m_Seed(pSeed),
    m_Shape(pShape) {
}

void RandomNormal::print(std::ostream& pOS) const
{
}
