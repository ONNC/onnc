//===- HardSigmoid.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/HardSigmoid.h>

using namespace onnc;

char HardSigmoid::ID = 0;

//===----------------------------------------------------------------------===//
// HardSigmoid
//===----------------------------------------------------------------------===//
HardSigmoid::HardSigmoid()
  : ComputeOperator("HardSigmoid", ID),
    m_Alpha(),
    m_Beta() {
}

HardSigmoid::HardSigmoid(const FloatAttr& pAlpha,
                         const FloatAttr& pBeta)
  : ComputeOperator("HardSigmoid", ID),
    m_Alpha(pAlpha),
    m_Beta(pBeta) {
}

void HardSigmoid::print(std::ostream& pOS) const
{
}

bool HardSigmoid::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
