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
    m_Alpha() {
}

LeakyRelu::LeakyRelu(const FloatAttr& pAlpha)
  : ComputeOperator("LeakyRelu", ID),
    m_Alpha(pAlpha) {
}

void LeakyRelu::print(std::ostream& pOS) const
{
}

bool LeakyRelu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
