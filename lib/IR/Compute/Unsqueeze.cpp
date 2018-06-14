//===- Unsqueeze.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Unsqueeze.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Unsqueeze
//===----------------------------------------------------------------------===//
Unsqueeze::Unsqueeze()
  : ComputeOperator("Unsqueeze"),
    m_Axes() {
}

Unsqueeze::Unsqueeze(const IntsAttr& pAxes)
  : ComputeOperator("Unsqueeze"),
    m_Axes(pAxes) {
}

void Unsqueeze::print(std::ostream& pOS) const
{
}
