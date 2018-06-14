//===- Scale.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Scale.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Scale
//===----------------------------------------------------------------------===//
Scale::Scale()
  : ComputeOperator("Scale"),
    m_Scale() {
}

Scale::Scale(const FloatAttr& pScale)
  : ComputeOperator("Scale"),
    m_Scale(pScale) {
}

void Scale::print(std::ostream& pOS) const
{
}
