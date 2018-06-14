//===- LpNormalization.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/LpNormalization.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LpNormalization
//===----------------------------------------------------------------------===//
LpNormalization::LpNormalization()
  : ComputeOperator("LpNormalization"),
    m_Axis(),
    m_P() {
}

LpNormalization::LpNormalization(const IntAttr& pAxis,
                                 const IntAttr& pP)
  : ComputeOperator("LpNormalization"),
    m_Axis(pAxis),
    m_P(pP) {
}

void LpNormalization::print(std::ostream& pOS) const
{
}
