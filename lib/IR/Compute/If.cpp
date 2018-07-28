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
If::If()
  : ComputeOperator("If", ID),
    m_ElseBranch(),
    m_ThenBranch() {
}

If::If(const GraphAttr& pElseBranch,
       const GraphAttr& pThenBranch)
  : ComputeOperator("If", ID),
    m_ElseBranch(pElseBranch),
    m_ThenBranch(pThenBranch) {
}

void If::print(std::ostream& pOS) const
{
}

bool If::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
