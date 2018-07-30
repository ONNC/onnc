//===- ReduceProd.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceProd.h>

using namespace onnc;

char ReduceProd::ID = 0;

//===----------------------------------------------------------------------===//
// ReduceProd
//===----------------------------------------------------------------------===//
ReduceProd::ReduceProd()
  : ComputeOperator("ReduceProd", ID),
    m_Axes(),
    m_Keepdims(1) {
}

ReduceProd::ReduceProd(const IntsAttr& pAxes,
                       const IntAttr& pKeepdims)
  : ComputeOperator("ReduceProd", ID),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

ReduceProd::ReduceProd(const ReduceProd& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axes(pCopy.getAxes()),
    m_Keepdims(pCopy.getKeepdims()) {
}

void ReduceProd::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxes() << ", " << getKeepdims() << ">";
}

bool ReduceProd::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
