//===- ReduceL2.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceL2.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReduceL2
//===----------------------------------------------------------------------===//
ReduceL2::ReduceL2()
  : ComputeOperator("ReduceL2"),
    m_Axes(),
    m_Keepdims() {
}

ReduceL2::ReduceL2(const IntsAttr& pAxes,
                   const IntAttr& pKeepdims)
  : ComputeOperator("ReduceL2"),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceL2::print(std::ostream& pOS) const
{
}
