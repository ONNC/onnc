//===- Pad.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Pad.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Pad
//===----------------------------------------------------------------------===//
Pad::Pad()
  : ComputeOperator("Pad"),
    m_Mode(),
    m_Pads(),
    m_Value() {
}

Pad::Pad(const StringAttr& pMode,
         const IntsAttr& pPads,
         const FloatAttr& pValue)
  : ComputeOperator("Pad"),
    m_Mode(pMode),
    m_Pads(pPads),
    m_Value(pValue) {
}

void Pad::print(std::ostream& pOS) const
{
}
