//===- ReduceLogSumExp.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceLogSumExp.h>

using namespace onnc;

char ReduceLogSumExp::ID = 0;

//===----------------------------------------------------------------------===//
// ReduceLogSumExp
//===----------------------------------------------------------------------===//
ReduceLogSumExp::ReduceLogSumExp()
  : ComputeOperator("ReduceLogSumExp", ID),
    m_Axes(),
    m_Keepdims() {
}

ReduceLogSumExp::ReduceLogSumExp(const IntsAttr& pAxes,
                                 const IntAttr& pKeepdims)
  : ComputeOperator("ReduceLogSumExp", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceLogSumExp::print(std::ostream& pOS) const
{
}

bool ReduceLogSumExp::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
