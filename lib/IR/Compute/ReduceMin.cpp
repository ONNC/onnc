//===- ReduceMin.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceMin.h>

using namespace onnc;

char ReduceMin::ID = 0;

//===----------------------------------------------------------------------===//
// ReduceMin
//===----------------------------------------------------------------------===//
ReduceMin::ReduceMin()
  : ComputeOperator("ReduceMin", ID),
    m_Axes(),
    m_Keepdims(1) {
}

ReduceMin::ReduceMin(const IntsAttr& pAxes,
                     const IntAttr& pKeepdims)
  : ComputeOperator("ReduceMin", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

ReduceMin::ReduceMin(const ReduceMin& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axes(pCopy.getAxes()),
    m_Keepdims(pCopy.getKeepdims()) {
}

void ReduceMin::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxes() << ", " << getKeepdims() << ">";
}

bool ReduceMin::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
