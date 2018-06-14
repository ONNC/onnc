//===- ReduceL1.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceL1.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReduceL1
//===----------------------------------------------------------------------===//
ReduceL1::ReduceL1()
  : ComputeOperator("ReduceL1"),
    m_Axes(),
    m_Keepdims() {
}

ReduceL1::ReduceL1(const IntsAttr& pAxes,
                   const IntAttr& pKeepdims)
  : ComputeOperator("ReduceL1"),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceL1::print(std::ostream& pOS) const
{
}
