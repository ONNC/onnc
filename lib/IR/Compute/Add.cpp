//===- Add.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Add.h>

using namespace onnc;

char Add::ID = 0;

//===----------------------------------------------------------------------===//
// Add
//===----------------------------------------------------------------------===//
Add::Add()
  : ComputeOperator("Add", ID),
    m_Axis(0),
    m_Broadcast(0) {
}

Add::Add(const IntAttr& pAxis,
         const IntAttr& pBroadcast)
  : ComputeOperator("Add", ID),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

Add::Add(const Add& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axis(pCopy.getAxis()),
    m_Broadcast(pCopy.getBroadcast()) {
}

void Add::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxis() << ", " << getBroadcast() << ">";
}

bool Add::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
