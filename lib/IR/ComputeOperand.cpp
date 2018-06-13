//===- ComputeOperand.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/ComputeOperand.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ComputeOperand
//===----------------------------------------------------------------------===//
ComputeOperand::ComputeOperand()
  : m_pValue(nullptr) {
}

ComputeOperand::ComputeOperand(onnc::Value& pValue)
  : m_pValue(&pValue) {
}
