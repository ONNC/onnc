//===- Pad.cpp ------------------------------------------------===//
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
Pad::Pad()
  : ComputeOperator("Pad", ID),
    m_Mode(),
    m_Pads(),
    m_Value() {
}

Pad::Pad(const StringAttr& pMode,
         const IntsAttr& pPads,
         const FloatAttr& pValue)
  : ComputeOperator("Pad", ID),
    m_Mode(pMode),
    m_Pads(pPads),
    m_Value(pValue) {
}

void Pad::print(std::ostream& pOS) const
{
}

bool Pad::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
