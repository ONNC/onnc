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
    m_Alpha() {
}

ThresholdedRelu::ThresholdedRelu(const FloatAttr& pAlpha)
  : ComputeOperator("ThresholdedRelu", ID),
    m_Alpha(pAlpha) {
}

void ThresholdedRelu::print(std::ostream& pOS) const
{
}

bool ThresholdedRelu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
