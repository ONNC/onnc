//===- Pow.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Pow.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Pow
//===----------------------------------------------------------------------===//
Pow::Pow()
  : ComputeOperator("Pow"),
    m_Axis(),
    m_Broadcast() {
}

Pow::Pow(const IntAttr& pAxis,
         const IntAttr& pBroadcast)
  : ComputeOperator("Pow"),
    m_Axis(pAxis),
    m_Broadcast(pBroadcast) {
}

void Pow::print(std::ostream& pOS) const
{
}
