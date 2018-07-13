//===- Softmax.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Softmax.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Softmax
//===----------------------------------------------------------------------===//
Softmax::Softmax()
  : ComputeOperator("Softmax"),
    m_Axis(1) {
}

Softmax::Softmax(const IntAttr& pAxis)
  : ComputeOperator("Softmax"),
    m_Axis(pAxis) {
}

void Softmax::print(std::ostream& pOS) const
{
}
