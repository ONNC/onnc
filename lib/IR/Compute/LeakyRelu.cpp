//===- LeakyRelu.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/LeakyRelu.h>

using namespace onnc;

char LeakyRelu::ID = 0;

//===----------------------------------------------------------------------===//
// LeakyRelu
//===----------------------------------------------------------------------===//
LeakyRelu::LeakyRelu()
  : ComputeOperator("LeakyRelu", ID),
    m_Alpha(0.00999999977648) {
}

LeakyRelu::LeakyRelu(const FloatAttr& pAlpha)
  : ComputeOperator("LeakyRelu", ID),
    m_Alpha(pAlpha) {
}

LeakyRelu::LeakyRelu(const LeakyRelu& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Alpha(pCopy.getAlpha()) {
}

void LeakyRelu::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAlpha() << ">";
}

bool LeakyRelu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
