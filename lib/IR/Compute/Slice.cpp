//===- Slice.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Slice.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Slice
//===----------------------------------------------------------------------===//
Slice::Slice()
  : ComputeOperator("Slice"),
    m_Axes(),
    m_Ends(),
    m_Starts() {
}

Slice::Slice(const IntsAttr& pAxes,
             const IntsAttr& pEnds,
             const IntsAttr& pStarts)
  : ComputeOperator("Slice"),
    m_Axes(pAxes),
    m_Ends(pEnds),
    m_Starts(pStarts) {
}

void Slice::print(std::ostream& pOS) const
{
}
