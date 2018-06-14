//===- Mul.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Mul.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Mul
//===----------------------------------------------------------------------===//
Mul::Mul()
  : ComputeOperator("Mul"),
    m_Axis(),
    m_Broadcast() {
}

Mul::Mul(const IntAttr& pAxis,
         const IntAttr& pBroadcast)
  : ComputeOperator("Mul"),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Mul::print(std::ostream& pOS) const
{
}
