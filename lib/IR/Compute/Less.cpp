//===- Less.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Less.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Less
//===----------------------------------------------------------------------===//
Less::Less()
  : ComputeOperator("Less"),
    m_Axis(),
    m_Broadcast() {
}

Less::Less(const IntAttr& pAxis,
           const IntAttr& pBroadcast)
  : ComputeOperator("Less"),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Less::print(std::ostream& pOS) const
{
}
