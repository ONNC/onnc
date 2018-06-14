//===- Concat.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Concat.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Concat
//===----------------------------------------------------------------------===//
Concat::Concat()
  : ComputeOperator("Concat"),
    m_Axis() {
}

Concat::Concat(const IntAttr& pAxis)
  : ComputeOperator("Concat"),
    m_Axis(pAxis) {
}

void Concat::print(std::ostream& pOS) const
{
}
