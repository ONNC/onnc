//===- Upsample.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Upsample.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Upsample
//===----------------------------------------------------------------------===//
Upsample::Upsample(const FloatAttr& pHeightScale, const FloatAttr& pWidthScale)
  : ComputeOperator("Upsample"),
    m_HeightScale(pHeightScale),
    m_Mode("nearest"),
    m_WidthScale(pWidthScale) {
}

Upsample::Upsample(const FloatAttr& pHeightScale,
                   const StringAttr& pMode,
                   const FloatAttr& pWidthScale)
  : ComputeOperator("Upsample"),
    m_HeightScale(pHeightScale),
    m_Mode(pMode),
    m_WidthScale(pWidthScale) {
}

void Upsample::print(std::ostream& pOS) const
{
}
