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
    m_Axis(),
    m_Split() {
}

Split::Split(const IntAttr& pAxis,
             const IntsAttr& pSplit)
  : ComputeOperator("Split", ID),
    m_Axis(pAxis),
    m_Split(pSplit) {
}

void Split::print(std::ostream& pOS) const
{
}

bool Split::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
