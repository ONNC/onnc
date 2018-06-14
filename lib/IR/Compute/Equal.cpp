//===- Equal.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Equal.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Equal
//===----------------------------------------------------------------------===//
Equal::Equal()
  : ComputeOperator("Equal"),
    m_Axis(),
    m_Broadcast() {
}

Equal::Equal(const IntAttr& pAxis,
             const IntAttr& pBroadcast)
  : ComputeOperator("Equal"),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Equal::print(std::ostream& pOS) const
{
}
