//===- Or.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Or.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Or
//===----------------------------------------------------------------------===//
Or::Or()
  : ComputeOperator("Or"),
    m_Axis(),
    m_Broadcast() {
}

Or::Or(const IntAttr& pAxis,
       const IntAttr& pBroadcast)
  : ComputeOperator("Or"),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Or::print(std::ostream& pOS) const
{
}
