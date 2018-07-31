//===- ComputeRegOperand.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/ComputeRegOperand.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ComputeRegOperand
//===----------------------------------------------------------------------===//
ComputeRegOperand::ComputeRegOperand()
  : ComputeOperand(kRegOperand) {
}

ComputeRegOperand::~ComputeRegOperand()
{
}
