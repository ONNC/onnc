//===- LpNormalization.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/LpNormalization.h>

using namespace onnc;

char LpNormalization::ID = 0;

//===----------------------------------------------------------------------===//
// LpNormalization
//===----------------------------------------------------------------------===//
LpNormalization::LpNormalization()
  : ComputeOperator("LpNormalization", ID),
    m_Axis(-1),
    m_P(2) {
}

LpNormalization::LpNormalization(const IntAttr& pAxis,
                                 const IntAttr& pP)
  : ComputeOperator("LpNormalization", ID),
    m_Axis(pAxis),
    m_P(pP) {
}

LpNormalization::LpNormalization(const LpNormalization& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axis(pCopy.getAxis()),
    m_P(pCopy.getP()) {
}

void LpNormalization::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxis() << ", " << getP() << ">";
}

bool LpNormalization::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
