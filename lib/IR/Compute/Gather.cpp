//===- Gather.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Gather.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Gather
//===----------------------------------------------------------------------===//
Gather::Gather()
  : ComputeOperator("Gather"),
    m_Axis() {
}

Gather::Gather(const IntAttr& pAxis)
  : ComputeOperator("Gather"),
    m_Axis(pAxis) {
}

void Gather::print(std::ostream& pOS) const
{
}
