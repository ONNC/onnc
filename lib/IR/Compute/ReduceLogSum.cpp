//===- ReduceLogSum.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceLogSum.h>

using namespace onnc;

char ReduceLogSum::ID = 0;

//===----------------------------------------------------------------------===//
// ReduceLogSum
//===----------------------------------------------------------------------===//
ReduceLogSum::ReduceLogSum()
  : ComputeOperator("ReduceLogSum", ID),
    m_Axes(),
    m_Keepdims(1) {
}

ReduceLogSum::ReduceLogSum(const IntsAttr& pAxes,
                           const IntAttr& pKeepdims)
  : ComputeOperator("ReduceLogSum", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

ReduceLogSum::ReduceLogSum(const ReduceLogSum& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axes(pCopy.getAxes()),
    m_Keepdims(pCopy.getKeepdims()) {
}

void ReduceLogSum::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxes() << ", " << getKeepdims() << ">";
}

bool ReduceLogSum::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
