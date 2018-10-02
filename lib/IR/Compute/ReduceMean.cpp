//===- ReduceMean.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceMean.h>

using namespace onnc;

char ReduceMean::ID = 0;

//===----------------------------------------------------------------------===//
// ReduceMean
//===----------------------------------------------------------------------===//
ReduceMean::ReduceMean()
  : ComputeOperator("ReduceMean", ID),
    m_Axes(),
    m_Keepdims(1) {
}

ReduceMean::ReduceMean(const IntsAttr& pAxes,
                       const IntAttr& pKeepdims)
  : ComputeOperator("ReduceMean", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

ReduceMean::ReduceMean(const ReduceMean& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axes(pCopy.getAxes()),
    m_Keepdims(pCopy.getKeepdims()) {
}

void ReduceMean::printAttributes(std::ostream& pOS) const
{
  pOS << '<' << "axes: " << getAxes() << ", " "keepdims: " << getKeepdims()<< '>';
}

bool ReduceMean::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
