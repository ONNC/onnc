//===- ArgMax.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ArgMax.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ArgMax
//===----------------------------------------------------------------------===//
ArgMax::ArgMax()
  : ComputeOperator("ArgMax"),
    m_Axis(),
    m_Keepdims() {
}

ArgMax::ArgMax(const IntAttr& pAxis,
               const IntAttr& pKeepdims)
  : ComputeOperator("ArgMax"),
    m_Axis(pAxis),
    m_Keepdims(pKeepdims) {
}

void ArgMax::print(std::ostream& pOS) const
{
}
