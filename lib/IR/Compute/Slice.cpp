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
Slice::Slice()
  : ComputeOperator("Slice", ID),
    m_Axes(),
    m_Ends(),
    m_Starts() {
}

Slice::Slice(const IntsAttr& pAxes,
             const IntsAttr& pEnds,
             const IntsAttr& pStarts)
  : ComputeOperator("Slice", ID),
    m_Axes(pAxes),
    m_Ends(pEnds),
    m_Starts(pStarts) {
}

void Slice::print(std::ostream& pOS) const
{
}

bool Slice::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
