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
    m_Epsilon() {
}

InstanceNormalization::InstanceNormalization(const FloatAttr& pEpsilon)
  : ComputeOperator("InstanceNormalization", ID),
    m_Epsilon(pEpsilon) {
}

void InstanceNormalization::print(std::ostream& pOS) const
{
}

bool InstanceNormalization::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
