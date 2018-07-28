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
    m_Axis(),
    m_P() {
}

LpNormalization::LpNormalization(const IntAttr& pAxis,
                                 const IntAttr& pP)
  : ComputeOperator("LpNormalization", ID),
    m_Axis(pAxis),
    m_P(pP) {
}

void LpNormalization::print(std::ostream& pOS) const
{
}

bool LpNormalization::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
