//===- ReduceLogSum.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceLogSum.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReduceLogSum
//===----------------------------------------------------------------------===//
ReduceLogSum::ReduceLogSum()
  : ComputeOperator("ReduceLogSum"),
    m_Axes(),
    m_Keepdims() {
}

ReduceLogSum::ReduceLogSum(const IntsAttr& pAxes,
                           const IntAttr& pKeepdims)
  : ComputeOperator("ReduceLogSum"),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceLogSum::print(std::ostream& pOS) const
{
}
