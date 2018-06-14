//===- Hardmax.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Hardmax.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Hardmax
//===----------------------------------------------------------------------===//
Hardmax::Hardmax()
  : ComputeOperator("Hardmax"),
    m_Axis() {
}

Hardmax::Hardmax(const IntAttr& pAxis)
  : ComputeOperator("Hardmax"),
    m_Axis(pAxis) {
}

void Hardmax::print(std::ostream& pOS) const
{
}
