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
    m_Keepdims(1) {
}

ReduceSumSquare::ReduceSumSquare(const IntsAttr& pAxes,
                                 const IntAttr& pKeepdims)
  : ComputeOperator("ReduceSumSquare", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

ReduceSumSquare::ReduceSumSquare(const ReduceSumSquare& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axes(pCopy.getAxes()),
    m_Keepdims(pCopy.getKeepdims()) {
}

void ReduceSumSquare::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxes() << ", " << getKeepdims() << ">";
}

bool ReduceSumSquare::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
