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
  : m_pValue(nullptr), m_Residence(kUnknownResidence) {
}

ComputeOperand::ComputeOperand(onnc::Value& pValue, Residence pResidence)
  : m_pValue(&pValue), m_Residence(pResidence) {
}

ComputeOperand::~ComputeOperand()
{
  // do nothing
}
