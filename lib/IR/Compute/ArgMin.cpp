//===- ArgMin.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ArgMin.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ArgMin
//===----------------------------------------------------------------------===//
ArgMin::ArgMin()
  : ComputeOperator("ArgMin"),
    m_Axis(),
    m_Keepdims() {
}

ArgMin::ArgMin(const IntAttr& pAxis,
               const IntAttr& pKeepdims)
  : ComputeOperator("ArgMin"),
    m_Axis(pAxis),
    m_Keepdims(pKeepdims) {
}

void ArgMin::print(std::ostream& pOS) const
{
}
