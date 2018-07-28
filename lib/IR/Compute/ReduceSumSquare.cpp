//===- ReduceSumSquare.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceSumSquare.h>

using namespace onnc;

char ReduceSumSquare::ID = 0;

//===----------------------------------------------------------------------===//
// ReduceSumSquare
//===----------------------------------------------------------------------===//
ReduceSumSquare::ReduceSumSquare()
  : ComputeOperator("ReduceSumSquare", ID),
    m_Axes(),
    m_Keepdims() {
}

ReduceSumSquare::ReduceSumSquare(const IntsAttr& pAxes,
                                 const IntAttr& pKeepdims)
  : ComputeOperator("ReduceSumSquare", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceSumSquare::print(std::ostream& pOS) const
{
}

bool ReduceSumSquare::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
