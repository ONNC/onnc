//===- MeanVarianceNormalization.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/MeanVarianceNormalization.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// MeanVarianceNormalization
//===----------------------------------------------------------------------===//
MeanVarianceNormalization::MeanVarianceNormalization()
  : ComputeOperator("MeanVarianceNormalization"),
    m_AcrossChannels(),
    m_NormalizeVariance() {
}

MeanVarianceNormalization::MeanVarianceNormalization(const IntAttr& pAcrossChannels,
                                                     const IntAttr& pNormalizeVariance)
  : ComputeOperator("MeanVarianceNormalization"),
    m_AcrossChannels(pAcrossChannels),
    m_NormalizeVariance(pNormalizeVariance) {
}

void MeanVarianceNormalization::print(std::ostream& pOS) const
{
}
