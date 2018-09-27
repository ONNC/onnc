//===- Unsqueeze.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Unsqueeze.h>

using namespace onnc;

char Unsqueeze::ID = 0;

//===----------------------------------------------------------------------===//
// Unsqueeze
//===----------------------------------------------------------------------===//
Unsqueeze::Unsqueeze(const IntsAttr& pAxes)
  : ComputeOperator("Unsqueeze", ID),
    m_Axes(pAxes) {
}



Unsqueeze::Unsqueeze(const Unsqueeze& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axes(pCopy.getAxes()) {
}

void Unsqueeze::printAttributes(std::ostream& pOS) const
{
  pOS << '<' << "axes: " << getAxes()<< '>';
}

bool Unsqueeze::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
