//===- Pad.cpp ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Pad.h>

using namespace onnc;

char Pad::ID = 0;

//===----------------------------------------------------------------------===//
// Pad
//===----------------------------------------------------------------------===//
Pad::Pad(const IntsAttr& pPads)
  : ComputeOperator("Pad", ID),
    m_Mode("constant"),
    m_Pads(pPads),
    m_Value(0.0) {
}

Pad::Pad(const StringAttr& pMode,
         const IntsAttr& pPads,
         const FloatAttr& pValue)
  : ComputeOperator("Pad", ID),
    m_Mode(pMode),
    m_Pads(pPads),
    m_Value(pValue) {
}

Pad::Pad(const Pad& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Mode(pCopy.getMode()),
    m_Pads(pCopy.getPads()),
    m_Value(pCopy.getValue()) {
}

void Pad::printAttributes(std::ostream& pOS) const
{
  pOS << '<' << "mode: " << getMode() << ", " "pads: " << getPads() << ", " "value: " << getValue()<< '>';
}

bool Pad::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
