//===- Dropout.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Dropout.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Dropout
//===----------------------------------------------------------------------===//
Dropout::Dropout()
  : ComputeOperator("Dropout"),
    m_IsTest(),
    m_Ratio() {
}

Dropout::Dropout(const IntAttr& pIsTest,
                 const FloatAttr& pRatio)
  : ComputeOperator("Dropout"),
    m_IsTest(pIsTest),
    m_Ratio(pRatio) {
}

void Dropout::print(std::ostream& pOS) const
{
}
