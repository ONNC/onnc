//===- ConvTranspose.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ConvTranspose.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ConvTranspose
//===----------------------------------------------------------------------===//
ConvTranspose::ConvTranspose()
  : ComputeOperator("ConvTranspose"),
    m_AutoPad(),
    m_Dilations(),
    m_Group(),
    m_KernelShape(),
    m_OutputPadding(),
    m_OutputShape(),
    m_Pads(),
    m_Strides() {
}

ConvTranspose::ConvTranspose(const StringAttr& pAutoPad,
                             const IntsAttr& pDilations,
                             const IntAttr& pGroup,
                             const IntsAttr& pKernelShape,
                             const IntsAttr& pOutputPadding,
                             const IntsAttr& pOutputShape,
                             const IntsAttr& pPads,
                             const IntsAttr& pStrides)
  : ComputeOperator("ConvTranspose"),
    m_AutoPad(pAutoPad),
    m_Dilations(pDilations),
    m_Group(pGroup),
    m_KernelShape(pKernelShape),
    m_OutputPadding(pOutputPadding),
    m_OutputShape(pOutputShape),
    m_Pads(pPads),
    m_Strides(pStrides) {
}

void ConvTranspose::print(std::ostream& pOS) const
{
}
