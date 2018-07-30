//===- ReduceL2.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceL2.h>

using namespace onnc;

char ReduceL2::ID = 0;

//===----------------------------------------------------------------------===//
// ReduceL2
//===----------------------------------------------------------------------===//
ReduceL2::ReduceL2()
  : ComputeOperator("ReduceL2", ID),
    m_Axes(),
    m_Keepdims(1) {
}

ReduceL2::ReduceL2(const IntsAttr& pAxes,
                   const IntAttr& pKeepdims)
  : ComputeOperator("ReduceL2", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

ReduceL2::ReduceL2(const ReduceL2& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axes(pCopy.getAxes()),
    m_Keepdims(pCopy.getKeepdims()) {
}

void ReduceL2::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxes() << ", " << getKeepdims() << ">";
}

bool ReduceL2::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
