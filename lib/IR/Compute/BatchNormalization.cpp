//===- BatchNormalization.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/BatchNormalization.h>

using namespace onnc;

char BatchNormalization::ID = 0;

//===----------------------------------------------------------------------===//
// BatchNormalization
//===----------------------------------------------------------------------===//
BatchNormalization::BatchNormalization()
  : ComputeOperator("BatchNormalization", ID),
    m_Epsilon(9.99999974738e-06),
    m_Momentum(0.899999976158),
    m_Spatial(1) {
}

BatchNormalization::BatchNormalization(const FloatAttr& pEpsilon,
                                       const FloatAttr& pMomentum,
                                       const IntAttr& pSpatial)
  : ComputeOperator("BatchNormalization", ID),
    m_Epsilon(pEpsilon),
    m_Momentum(pMomentum),
    m_Spatial(pSpatial) {
}

BatchNormalization::BatchNormalization(const BatchNormalization& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Epsilon(pCopy.getEpsilon()),
    m_Momentum(pCopy.getMomentum()),
    m_Spatial(pCopy.getSpatial()) {
}

void BatchNormalization::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getEpsilon() << ", " << getMomentum() << ", " << getSpatial() << ">";
}

bool BatchNormalization::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
