//===- Add.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Add.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Add
//===----------------------------------------------------------------------===//
Add::Add()
  : ComputeOperator("Add"),
    m_Axis(0),
    m_Broadcast(0) {
}

Add::Add(const IntAttr& pAxis,
         const IntAttr& pBroadcast)
  : ComputeOperator("Add"),
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
