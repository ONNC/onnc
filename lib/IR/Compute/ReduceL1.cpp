//===- ReduceL1.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceL1.h>

using namespace onnc;

char ReduceL1::ID = 0;

//===----------------------------------------------------------------------===//
// ReduceL1
//===----------------------------------------------------------------------===//
ReduceL1::ReduceL1()
  : ComputeOperator("ReduceL1", ID),
    m_Axes(),
    m_Keepdims() {
}

ReduceL1::ReduceL1(const IntsAttr& pAxes,
                   const IntAttr& pKeepdims)
  : ComputeOperator("ReduceL1", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceL1::print(std::ostream& pOS) const
{
}

bool ReduceL1::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
