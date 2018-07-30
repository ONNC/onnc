//===- ReduceMax.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceMax.h>

using namespace onnc;

char ReduceMax::ID = 0;

//===----------------------------------------------------------------------===//
// ReduceMax
//===----------------------------------------------------------------------===//
ReduceMax::ReduceMax()
  : ComputeOperator("ReduceMax", ID),
    m_Axes(),
    m_Keepdims(1) {
}

ReduceMax::ReduceMax(const IntsAttr& pAxes,
                     const IntAttr& pKeepdims)
  : ComputeOperator("ReduceMax", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

ReduceMax::ReduceMax(const ReduceMax& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axes(pCopy.getAxes()),
    m_Keepdims(pCopy.getKeepdims()) {
}

void ReduceMax::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxes() << ", " << getKeepdims() << ">";
}

bool ReduceMax::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
