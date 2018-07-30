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
    m_AcrossChannels(0),
    m_NormalizeVariance(1) {
}

MeanVarianceNormalization::MeanVarianceNormalization(const IntAttr& pAcrossChannels,
                                                     const IntAttr& pNormalizeVariance)
  : ComputeOperator("MeanVarianceNormalization", ID),
    m_AcrossChannels(pAcrossChannels),
    m_NormalizeVariance(pNormalizeVariance) {
}

MeanVarianceNormalization::MeanVarianceNormalization(const MeanVarianceNormalization& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_AcrossChannels(pCopy.getAcrossChannels()),
    m_NormalizeVariance(pCopy.getNormalizeVariance()) {
}

void MeanVarianceNormalization::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAcrossChannels() << ", " << getNormalizeVariance() << ">";
}

bool MeanVarianceNormalization::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
