//===- Slice.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Slice.h>

using namespace onnc;

char Slice::ID = 0;

//===----------------------------------------------------------------------===//
// Slice
//===----------------------------------------------------------------------===//
Slice::Slice(const IntsAttr& pEnds, const IntsAttr& pStarts)
  : ComputeOperator("Slice", ID),
    m_Axes(),
    m_Ends(pEnds),
    m_Starts(pStarts) {
}

Slice::Slice(const IntsAttr& pAxes,
             const IntsAttr& pEnds,
             const IntsAttr& pStarts)
  : ComputeOperator("Slice", ID),
    m_Axes(pAxes),
    m_Ends(pEnds),
    m_Starts(pStarts) {
}

Slice::Slice(const Slice& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axes(pCopy.getAxes()),
    m_Ends(pCopy.getEnds()),
    m_Starts(pCopy.getStarts()) {
}

void Slice::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxes() << ", " << getEnds() << ", " << getStarts() << ">";
}

bool Slice::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
