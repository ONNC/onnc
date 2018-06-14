//===- Clip.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Clip.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Clip
//===----------------------------------------------------------------------===//
Clip::Clip()
  : ComputeOperator("Clip"),
    m_Max(),
    m_Min() {
}

Clip::Clip(const FloatAttr& pMax,
           const FloatAttr& pMin)
  : ComputeOperator("Clip"),
    m_Max(pMax),
    m_Min(pMin) {
}

void Clip::print(std::ostream& pOS) const
{
}
