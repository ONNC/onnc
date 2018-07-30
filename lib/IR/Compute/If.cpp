//===- If.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/If.h>

using namespace onnc;

char If::ID = 0;

//===----------------------------------------------------------------------===//
// If
//===----------------------------------------------------------------------===//
If::If(const GraphAttr& pElseBranch, const GraphAttr& pThenBranch)
  : ComputeOperator("If", ID),
    m_ElseBranch(pElseBranch),
    m_ThenBranch(pThenBranch) {
}



If::If(const If& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_ElseBranch(pCopy.getElseBranch()),
    m_ThenBranch(pCopy.getThenBranch()) {
}

void If::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getElseBranch() << ", " << getThenBranch() << ">";
}

bool If::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
