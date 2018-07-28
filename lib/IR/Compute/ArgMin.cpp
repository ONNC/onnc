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
    m_Axis(),
    m_Keepdims() {
}

ArgMin::ArgMin(const IntAttr& pAxis,
               const IntAttr& pKeepdims)
  : ComputeOperator("ArgMin", ID),
    m_Axis(pAxis),
    m_Keepdims(pKeepdims) {
}

void ArgMin::print(std::ostream& pOS) const
{
}

bool ArgMin::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
