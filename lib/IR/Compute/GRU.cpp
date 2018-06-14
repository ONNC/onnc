//===- GRU.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/GRU.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// GRU
//===----------------------------------------------------------------------===//
GRU::GRU()
  : ComputeOperator("GRU"),
    m_ActivationAlpha(),
    m_ActivationBeta(),
    m_Activations(),
    m_Clip(),
    m_Direction(),
    m_HiddenSize(),
    m_LinearBeforeReset(),
    m_OutputSequence() {
}

GRU::GRU(const FloatsAttr& pActivationAlpha,
         const FloatsAttr& pActivationBeta,
         const StringsAttr& pActivations,
         const FloatAttr& pClip,
         const StringAttr& pDirection,
         const IntAttr& pHiddenSize,
         const IntAttr& pLinearBeforeReset,
         const IntAttr& pOutputSequence)
  : ComputeOperator("GRU"),
    m_ActivationAlpha(pActivationAlpha),
    m_ActivationBeta(pActivationBeta),
    m_Activations(pActivations),
    m_Clip(pClip),
    m_Direction(pDirection),
    m_HiddenSize(pHiddenSize),
    m_LinearBeforeReset(pLinearBeforeReset),
    m_OutputSequence(pOutputSequence) {
}

void GRU::print(std::ostream& pOS) const
{
}
