//===- RandomUniformLike.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/RandomUniformLike.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// RandomUniformLike
//===----------------------------------------------------------------------===//
RandomUniformLike::RandomUniformLike()
  : ComputeOperator("RandomUniformLike"),
    m_Dtype(),
    m_High(),
    m_Low(),
    m_Seed() {
}

RandomUniformLike::RandomUniformLike(const IntAttr& pDtype,
                                     const FloatAttr& pHigh,
                                     const FloatAttr& pLow,
                                     const FloatAttr& pSeed)
  : ComputeOperator("RandomUniformLike"),
    m_Dtype(pDtype),
    m_High(pHigh),
    m_Low(pLow),
    m_Seed(pSeed) {
}

void RandomUniformLike::print(std::ostream& pOS) const
{
}
