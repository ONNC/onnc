//===- BatchNormalization.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/BatchNormalization.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// BatchNormalization
//===----------------------------------------------------------------------===//
BatchNormalization::BatchNormalization()
  : ComputeOperator("BatchNormalization"),
    m_Epsilon(1e-5f),
    m_IsTest(0),
    m_Momentum(0.9f),
    m_Spatial(1) {
}

BatchNormalization::BatchNormalization(const FloatAttr& pEpsilon,
                                       const IntAttr& pIsTest,
                                       const FloatAttr& pMomentum,
                                       const IntAttr& pSpatial)
  : ComputeOperator("BatchNormalization"),
    m_Epsilon(pEpsilon),
    m_IsTest(pIsTest),
    m_Momentum(pMomentum),
    m_Spatial(pSpatial) {
}

void BatchNormalization::print(std::ostream& pOS) const
{
}
