//===- ReduceSum.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceSum.h>

using namespace onnc;

char ReduceSum::ID = 0;

//===----------------------------------------------------------------------===//
// ReduceSum
//===----------------------------------------------------------------------===//
ReduceSum::ReduceSum()
  : ComputeOperator("ReduceSum", ID),
    m_Axes(),
    m_Keepdims(1) {
}

ReduceSum::ReduceSum(const IntsAttr& pAxes,
                     const IntAttr& pKeepdims)
  : ComputeOperator("ReduceSum", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

ReduceSum::ReduceSum(const ReduceSum& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axes(pCopy.getAxes()),
    m_Keepdims(pCopy.getKeepdims()) {
}

void ReduceSum::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxes() << ", " << getKeepdims() << ">";
}

bool ReduceSum::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
