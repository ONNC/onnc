//===- Div.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Div.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Div
//===----------------------------------------------------------------------===//
Div::Div()
  : ComputeOperator("Div"),
    m_Axis(),
    m_Broadcast() {
}

Div::Div(const IntAttr& pAxis,
         const IntAttr& pBroadcast)
  : ComputeOperator("Div"),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Div::print(std::ostream& pOS) const
{
}
