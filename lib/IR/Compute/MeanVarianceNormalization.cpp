//===- MeanVarianceNormalization.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/MeanVarianceNormalization.h>

using namespace onnc;

char MeanVarianceNormalization::ID = 0;

//===----------------------------------------------------------------------===//
// MeanVarianceNormalization
//===----------------------------------------------------------------------===//
MeanVarianceNormalization::MeanVarianceNormalization()
  : ComputeOperator("MeanVarianceNormalization", ID),
    m_AcrossChannels(),
    m_NormalizeVariance() {
}

MeanVarianceNormalization::MeanVarianceNormalization(const IntAttr& pAcrossChannels,
                                                     const IntAttr& pNormalizeVariance)
  : ComputeOperator("MeanVarianceNormalization", ID),
    m_AcrossChannels(pAcrossChannels),
    m_NormalizeVariance(pNormalizeVariance) {
}

void MeanVarianceNormalization::print(std::ostream& pOS) const
{
}

bool MeanVarianceNormalization::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
