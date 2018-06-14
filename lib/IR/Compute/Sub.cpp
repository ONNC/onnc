//===- Sub.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Sub.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Sub
//===----------------------------------------------------------------------===//
Sub::Sub()
  : ComputeOperator("Sub"),
    m_Axis(),
    m_Broadcast() {
}

Sub::Sub(const IntAttr& pAxis,
         const IntAttr& pBroadcast)
  : ComputeOperator("Sub"),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Sub::print(std::ostream& pOS) const
{
}
