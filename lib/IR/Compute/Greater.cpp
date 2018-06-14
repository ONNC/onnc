//===- Greater.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Greater.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Greater
//===----------------------------------------------------------------------===//
Greater::Greater()
  : ComputeOperator("Greater"),
    m_Axis(),
    m_Broadcast() {
}

Greater::Greater(const IntAttr& pAxis,
                 const IntAttr& pBroadcast)
  : ComputeOperator("Greater"),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Greater::print(std::ostream& pOS) const
{
}
