//===- ReduceLogSumExp.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceLogSumExp.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReduceLogSumExp
//===----------------------------------------------------------------------===//
ReduceLogSumExp::ReduceLogSumExp()
  : ComputeOperator("ReduceLogSumExp"),
    m_Axes(),
    m_Keepdims() {
}

ReduceLogSumExp::ReduceLogSumExp(const IntsAttr& pAxes,
                                 const IntAttr& pKeepdims)
  : ComputeOperator("ReduceLogSumExp"),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceLogSumExp::print(std::ostream& pOS) const
{
}
