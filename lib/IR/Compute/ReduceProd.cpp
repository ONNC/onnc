//===- ReduceProd.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceProd.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReduceProd
//===----------------------------------------------------------------------===//
ReduceProd::ReduceProd()
  : ComputeOperator("ReduceProd"),
    m_Axes(),
    m_Keepdims() {
}

ReduceProd::ReduceProd(const IntsAttr& pAxes,
                       const IntAttr& pKeepdims)
  : ComputeOperator("ReduceProd"),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceProd::print(std::ostream& pOS) const
{
}
