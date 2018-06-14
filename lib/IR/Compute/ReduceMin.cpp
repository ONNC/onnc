//===- ReduceMin.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceMin.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReduceMin
//===----------------------------------------------------------------------===//
ReduceMin::ReduceMin()
  : ComputeOperator("ReduceMin"),
    m_Axes(),
    m_Keepdims() {
}

ReduceMin::ReduceMin(const IntsAttr& pAxes,
                     const IntAttr& pKeepdims)
  : ComputeOperator("ReduceMin"),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceMin::print(std::ostream& pOS) const
{
}
