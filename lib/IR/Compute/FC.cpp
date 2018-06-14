//===- FC.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/FC.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// FC
//===----------------------------------------------------------------------===//
FC::FC()
  : ComputeOperator("FC"),
    m_Axis(),
    m_AxisW() {
}

FC::FC(const IntAttr& pAxis,
       const IntAttr& pAxisW)
  : ComputeOperator("FC"),
    m_Axis(pAxis),
    m_AxisW(pAxisW) {
}

void FC::print(std::ostream& pOS) const
{
}
