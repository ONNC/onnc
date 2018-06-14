//===- ReduceSum.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceSum.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReduceSum
//===----------------------------------------------------------------------===//
ReduceSum::ReduceSum()
  : ComputeOperator("ReduceSum"),
    m_Axes(),
    m_Keepdims() {
}

ReduceSum::ReduceSum(const IntsAttr& pAxes,
                     const IntAttr& pKeepdims)
  : ComputeOperator("ReduceSum"),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceSum::print(std::ostream& pOS) const
{
}
