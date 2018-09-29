//===- Cast.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Cast.h>

using namespace onnc;

char Cast::ID = 0;

//===----------------------------------------------------------------------===//
// Cast
//===----------------------------------------------------------------------===//
Cast::Cast(const IntAttr& pTo)
  : ComputeOperator("Cast", ID),
    m_To(pTo) {
}



Cast::Cast(const Cast& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_To(pCopy.getTo()) {
}

void Cast::printAttributes(std::ostream& pOS) const
{
  pOS << '<' << "to: " << getTo()<< '>';
}

bool Cast::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
