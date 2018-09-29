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
    m_Keepdims(1) {
}

ReduceLogSumExp::ReduceLogSumExp(const IntsAttr& pAxes,
                                 const IntAttr& pKeepdims)
  : ComputeOperator("ReduceLogSumExp", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

ReduceLogSumExp::ReduceLogSumExp(const ReduceLogSumExp& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axes(pCopy.getAxes()),
    m_Keepdims(pCopy.getKeepdims()) {
}

void ReduceLogSumExp::printAttributes(std::ostream& pOS) const
{
  pOS << '<' << "axes: " << getAxes() << ", " "keepdims: " << getKeepdims()<< '>';
}

bool ReduceLogSumExp::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
