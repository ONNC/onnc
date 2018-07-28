//===- FC.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/FC.h>

using namespace onnc;

char FC::ID = 0;

//===----------------------------------------------------------------------===//
// FC
//===----------------------------------------------------------------------===//
FC::FC()
  : ComputeOperator("FC", ID),
    m_Axis(),
    m_AxisW() {
}

FC::FC(const IntAttr& pAxis,
       const IntAttr& pAxisW)
  : ComputeOperator("FC", ID),
    m_Axis(pAxis),
    m_AxisW(pAxisW) {
}

void FC::print(std::ostream& pOS) const
{
}

bool FC::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
