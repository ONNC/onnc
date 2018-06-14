//===- And.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/And.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// And
//===----------------------------------------------------------------------===//
And::And()
  : ComputeOperator("And"),
    m_Axis(),
    m_Broadcast() {
}

And::And(const IntAttr& pAxis,
         const IntAttr& pBroadcast)
  : ComputeOperator("And"),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void And::print(std::ostream& pOS) const
{
}
