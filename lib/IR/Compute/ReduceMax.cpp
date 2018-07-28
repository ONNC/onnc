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
    m_Keepdims() {
}

ReduceMax::ReduceMax(const IntsAttr& pAxes,
                     const IntAttr& pKeepdims)
  : ComputeOperator("ReduceMax", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceMax::print(std::ostream& pOS) const
{
}

bool ReduceMax::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
