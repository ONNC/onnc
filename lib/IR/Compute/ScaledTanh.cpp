//===- ScaledTanh.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ScaledTanh.h>

using namespace onnc;

char ScaledTanh::ID = 0;

//===----------------------------------------------------------------------===//
// ScaledTanh
//===----------------------------------------------------------------------===//
ScaledTanh::ScaledTanh()
  : ComputeOperator("ScaledTanh", ID),
    m_Alpha(),
    m_Beta() {
}

ScaledTanh::ScaledTanh(const FloatAttr& pAlpha,
                       const FloatAttr& pBeta)
  : ComputeOperator("ScaledTanh", ID),
    m_Alpha(pAlpha),
    m_Beta(pBeta) {
}

ScaledTanh::ScaledTanh(const ScaledTanh& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Alpha(pCopy.getAlpha()),
    m_Beta(pCopy.getBeta()) {
}

void ScaledTanh::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAlpha() << ", " << getBeta() << ">";
}

bool ScaledTanh::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
