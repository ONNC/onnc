//===- InstanceNormalization.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/InstanceNormalization.h>

using namespace onnc;

char InstanceNormalization::ID = 0;

//===----------------------------------------------------------------------===//
// InstanceNormalization
//===----------------------------------------------------------------------===//
InstanceNormalization::InstanceNormalization()
  : ComputeOperator("InstanceNormalization", ID),
    m_Epsilon(9.99999974738e-06) {
}

InstanceNormalization::InstanceNormalization(const FloatAttr& pEpsilon)
  : ComputeOperator("InstanceNormalization", ID),
    m_Epsilon(pEpsilon) {
}

InstanceNormalization::InstanceNormalization(const InstanceNormalization& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Epsilon(pCopy.getEpsilon()) {
}

void InstanceNormalization::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getEpsilon() << ">";
}

bool InstanceNormalization::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
