//===- InstanceNormalization.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/InstanceNormalization.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// InstanceNormalization
//===----------------------------------------------------------------------===//
InstanceNormalization::InstanceNormalization()
  : ComputeOperator("InstanceNormalization"),
    m_Epsilon() {
}

InstanceNormalization::InstanceNormalization(const FloatAttr& pEpsilon)
  : ComputeOperator("InstanceNormalization"),
    m_Epsilon(pEpsilon) {
}

void InstanceNormalization::print(std::ostream& pOS) const
{
}
