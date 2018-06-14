//===- LoopIndexTensor.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/LoopIndexTensor.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LoopIndexTensor
//===----------------------------------------------------------------------===//
LoopIndexTensor::LoopIndexTensor()
  : ComputeOperator("LoopIndexTensor"),
    m_Axis() {
}

LoopIndexTensor::LoopIndexTensor(const IntAttr& pAxis)
  : ComputeOperator("LoopIndexTensor"),
    m_Axis(pAxis) {
}

void LoopIndexTensor::print(std::ostream& pOS) const
{
}
