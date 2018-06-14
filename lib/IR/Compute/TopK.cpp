//===- TopK.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/TopK.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// TopK
//===----------------------------------------------------------------------===//
TopK::TopK()
  : ComputeOperator("TopK"),
    m_Axis(),
    m_K() {
}

TopK::TopK(const IntAttr& pAxis,
           const IntAttr& pK)
  : ComputeOperator("TopK"),
    m_Axis(pAxis),
    m_K(pK) {
}

void TopK::print(std::ostream& pOS) const
{
}
