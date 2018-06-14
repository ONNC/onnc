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
    m_Epsilon(),
    m_IsTest(),
    m_Momentum(),
    m_Spatial() {
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
