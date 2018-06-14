//===- Split.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Split.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Split
//===----------------------------------------------------------------------===//
Split::Split()
  : ComputeOperator("Split"),
    m_Axis(),
    m_Split() {
}

Split::Split(const IntAttr& pAxis,
             const IntsAttr& pSplit)
  : ComputeOperator("Split"),
    m_Axis(pAxis),
    m_Split(pSplit) {
}

void Split::print(std::ostream& pOS) const
{
}
