//===- ThresholdedRelu.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ThresholdedRelu.h>

using namespace onnc;

char ThresholdedRelu::ID = 0;

//===----------------------------------------------------------------------===//
// ThresholdedRelu
//===----------------------------------------------------------------------===//
ThresholdedRelu::ThresholdedRelu()
  : ComputeOperator("ThresholdedRelu", ID),
    m_Alpha(1.0) {
}

ThresholdedRelu::ThresholdedRelu(const FloatAttr& pAlpha)
  : ComputeOperator("ThresholdedRelu", ID),
    m_Alpha(pAlpha) {
}

ThresholdedRelu::ThresholdedRelu(const ThresholdedRelu& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Alpha(pCopy.getAlpha()) {
}

void ThresholdedRelu::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAlpha() << ">";
}

bool ThresholdedRelu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
