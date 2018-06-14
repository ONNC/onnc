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
    m_Axis(),
    m_Broadcast() {
}

Add::Add(const IntAttr& pAxis,
         const IntAttr& pBroadcast)
  : ComputeOperator("Add"),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Add::print(std::ostream& pOS) const
{
}
