//===- ReduceMax.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceMax.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReduceMax
//===----------------------------------------------------------------------===//
ReduceMax::ReduceMax()
  : ComputeOperator("ReduceMax"),
    m_Axes(),
    m_Keepdims() {
}

ReduceMax::ReduceMax(const IntsAttr& pAxes,
                     const IntAttr& pKeepdims)
  : ComputeOperator("ReduceMax"),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceMax::print(std::ostream& pOS) const
{
}
