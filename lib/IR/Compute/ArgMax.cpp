//===- ArgMax.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ArgMax.h>

using namespace onnc;

char ArgMax::ID = 0;

//===----------------------------------------------------------------------===//
// ArgMax
//===----------------------------------------------------------------------===//
ArgMax::ArgMax()
  : ComputeOperator("ArgMax", ID),
    m_Axis(0),
    m_Keepdims(1) {
}

ArgMax::ArgMax(const IntAttr& pAxis,
               const IntAttr& pKeepdims)
  : ComputeOperator("ArgMax", ID),
    m_Axis(pAxis),
    m_Keepdims(pKeepdims) {
}

ArgMax::ArgMax(const ArgMax& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axis(pCopy.getAxis()),
    m_Keepdims(pCopy.getKeepdims()) {
}

void ArgMax::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxis() << ", " << getKeepdims() << ">";
}

bool ArgMax::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
