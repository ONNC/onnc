//===- ReduceMean.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ReduceMean.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReduceMean
//===----------------------------------------------------------------------===//
ReduceMean::ReduceMean()
  : ComputeOperator("ReduceMean"),
    m_Axes(),
    m_Keepdims() {
}

ReduceMean::ReduceMean(const IntsAttr& pAxes,
                       const IntAttr& pKeepdims)
  : ComputeOperator("ReduceMean"),
    m_Axes(pAxes),
    m_Keepdims(pKeepdims) {
}

void ReduceMean::print(std::ostream& pOS) const
{
}
