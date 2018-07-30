//===- Split.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Split.h>

using namespace onnc;

char Split::ID = 0;

//===----------------------------------------------------------------------===//
// Split
//===----------------------------------------------------------------------===//
Split::Split()
  : ComputeOperator("Split", ID),
    m_Axis(0),
    m_Split() {
}

Split::Split(const IntAttr& pAxis,
             const IntsAttr& pSplit)
  : ComputeOperator("Split", ID),
    m_Axis(pAxis),
    m_Split(pSplit) {
}

Split::Split(const Split& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axis(pCopy.getAxis()),
    m_Split(pCopy.getSplit()) {
}

void Split::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxis() << ", " << getSplit() << ">";
}

bool Split::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
