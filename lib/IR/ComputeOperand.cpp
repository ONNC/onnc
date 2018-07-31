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
ComputeOperand::ComputeOperand(Kind pKind)
  : m_Kind(pKind), m_pValue(nullptr), m_Residence(kUnknownResidence) {
}

ComputeOperand::ComputeOperand(Kind pKind,
                               onnc::Value& pValue,
                               Residence pResidence)
  : m_Kind(pKind), m_pValue(&pValue), m_Residence(pResidence) {
}

ComputeOperand::~ComputeOperand()
{
  // do nothing
}
