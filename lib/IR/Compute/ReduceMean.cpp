//===- ReduceMean.cpp ------------------------------------------------===//
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
    m_Keepdims() {
}

ReduceMean::ReduceMean(const IntsAttr& pAxes,
                       const IntAttr& pKeepdims)
  : ComputeOperator("ReduceMean", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceMean::print(std::ostream& pOS) const
{
}

bool ReduceMean::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
