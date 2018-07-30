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
    m_Keepdims(1) {
}

ReduceL1::ReduceL1(const IntsAttr& pAxes,
                   const IntAttr& pKeepdims)
  : ComputeOperator("ReduceL1", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

ReduceL1::ReduceL1(const ReduceL1& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axes(pCopy.getAxes()),
    m_Keepdims(pCopy.getKeepdims()) {
}

void ReduceL1::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxes() << ", " << getKeepdims() << ">";
}

bool ReduceL1::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
