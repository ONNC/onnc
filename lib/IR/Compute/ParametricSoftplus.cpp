//===- ParametricSoftplus.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ParametricSoftplus.h>

using namespace onnc;

char ParametricSoftplus::ID = 0;

//===----------------------------------------------------------------------===//
// ParametricSoftplus
//===----------------------------------------------------------------------===//
ParametricSoftplus::ParametricSoftplus()
  : ComputeOperator("ParametricSoftplus", ID),
    m_Alpha(),
    m_Beta() {
}

ParametricSoftplus::ParametricSoftplus(const FloatAttr& pAlpha,
                                       const FloatAttr& pBeta)
  : ComputeOperator("ParametricSoftplus", ID),
    m_Alpha(pAlpha),
    m_Beta(pBeta) {
}

ParametricSoftplus::ParametricSoftplus(const ParametricSoftplus& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Alpha(pCopy.getAlpha()),
    m_Beta(pCopy.getBeta()) {
}

void ParametricSoftplus::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAlpha() << ", " << getBeta() << ">";
}

bool ParametricSoftplus::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
