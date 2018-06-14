//===- Squeeze.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Squeeze.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Squeeze
//===----------------------------------------------------------------------===//
Squeeze::Squeeze()
  : ComputeOperator("Squeeze"),
    m_Axes() {
}

Squeeze::Squeeze(const IntsAttr& pAxes)
  : ComputeOperator("Squeeze"),
    m_Axes(pAxes) {
}

void Squeeze::print(std::ostream& pOS) const
{
}
