//===- If.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/If.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// If
//===----------------------------------------------------------------------===//
If::If()
  : ComputeOperator("If"),
    m_ElseBranch(),
    m_ThenBranch() {
}

If::If(const GraphAttr& pElseBranch,
       const GraphAttr& pThenBranch)
  : ComputeOperator("If"),
    m_ElseBranch(pElseBranch),
    m_ThenBranch(pThenBranch) {
}

void If::print(std::ostream& pOS) const
{
}
