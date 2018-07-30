//===- ArgMin.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ArgMin.h>

using namespace onnc;

char ArgMin::ID = 0;

//===----------------------------------------------------------------------===//
// ArgMin
//===----------------------------------------------------------------------===//
ArgMin::ArgMin()
  : ComputeOperator("ArgMin", ID),
    m_Axis(0),
    m_Keepdims(1) {
}

ArgMin::ArgMin(const IntAttr& pAxis,
               const IntAttr& pKeepdims)
  : ComputeOperator("ArgMin", ID),
    m_Axis(pAxis),
    m_Keepdims(pKeepdims) {
}

ArgMin::ArgMin(const ArgMin& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axis(pCopy.getAxis()),
    m_Keepdims(pCopy.getKeepdims()) {
}

void ArgMin::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxis() << ", " << getKeepdims() << ">";
}

bool ArgMin::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
