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
    m_Axis(),
    m_Keepdims() {
}

ArgMax::ArgMax(const IntAttr& pAxis,
               const IntAttr& pKeepdims)
  : ComputeOperator("ArgMax", ID),
    m_Axis(pAxis),
    m_Keepdims(pKeepdims) {
}

void ArgMax::print(std::ostream& pOS) const
{
}

bool ArgMax::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
