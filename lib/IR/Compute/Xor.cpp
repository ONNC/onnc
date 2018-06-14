//===- Xor.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Xor.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Xor
//===----------------------------------------------------------------------===//
Xor::Xor()
  : ComputeOperator("Xor"),
    m_Axis(),
    m_Broadcast() {
}

Xor::Xor(const IntAttr& pAxis,
         const IntAttr& pBroadcast)
  : ComputeOperator("Xor"),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Xor::print(std::ostream& pOS) const
{
}
