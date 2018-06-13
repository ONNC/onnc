//===- Conv.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Conv.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Conv
//===----------------------------------------------------------------------===//
Conv::Conv()
  : ComputeOperator("Conv"),
    m_AutoPad(),
    m_Dilations(),
    m_Group(),
    m_KernelShape(),
    m_Strides(),
    m_Pads() {
}

Conv::Conv(const StringAttr& pAutoPad, const IntsAttr& pDilations,
           const IntAttr& pGroup, const IntsAttr& pKernelShape,
           const IntsAttr& pPads, const IntsAttr& pStrides)
  : ComputeOperator("Conv"),
    m_AutoPad(pAutoPad),
    m_Dilations(pDilations),
    m_Group(pGroup),
    m_KernelShape(pKernelShape),
    m_Pads(pPads),
    m_Strides(pStrides) {
}

void Conv::print(std::ostream& pOS) const
{
}
