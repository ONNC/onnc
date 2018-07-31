//===- ComputeMemOperand.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/ComputeMemOperand.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ComputeMemOperand
//===----------------------------------------------------------------------===//
ComputeMemOperand::ComputeMemOperand()
  : ComputeOperand(kMemOperand), m_Start(0), m_Length(0) {
}

ComputeMemOperand::ComputeMemOperand(uint32_t pStart, uint32_t pLength)
  : ComputeOperand(kMemOperand), m_Start(pStart), m_Length(pLength) {
}

ComputeMemOperand::ComputeMemOperand(onnc::Value& pValue, Residence pResidence)
  : ComputeOperand(kMemOperand, pValue, pResidence), m_Start(0), m_Length(0) {
}

ComputeMemOperand::~ComputeMemOperand()
{
  // do nothing
}
