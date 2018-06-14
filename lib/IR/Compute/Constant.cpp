//===- Constant.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Constant.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Constant
//===----------------------------------------------------------------------===//
Constant::Constant()
  : ComputeOperator("Constant"),
    m_Value() {
}

Constant::Constant(const TensorAttr& pValue)
  : ComputeOperator("Constant"),
    m_Value(pValue) {
}

void Constant::print(std::ostream& pOS) const
{
}
