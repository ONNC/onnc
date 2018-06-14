//===- Flatten.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Flatten.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Flatten
//===----------------------------------------------------------------------===//
Flatten::Flatten()
  : ComputeOperator("Flatten"),
    m_Axis() {
}

Flatten::Flatten(const IntAttr& pAxis)
  : ComputeOperator("Flatten"),
    m_Axis(pAxis) {
}

void Flatten::print(std::ostream& pOS) const
{
}
