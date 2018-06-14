//===- ReduceSumSquare.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceSumSquare.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReduceSumSquare
//===----------------------------------------------------------------------===//
ReduceSumSquare::ReduceSumSquare()
  : ComputeOperator("ReduceSumSquare"),
    m_Axes(),
    m_Keepdims() {
}

ReduceSumSquare::ReduceSumSquare(const IntsAttr& pAxes,
                                 const IntAttr& pKeepdims)
  : ComputeOperator("ReduceSumSquare"),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceSumSquare::print(std::ostream& pOS) const
{
}
