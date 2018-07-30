//===- Flatten.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Flatten.h>

using namespace onnc;

char Flatten::ID = 0;

//===----------------------------------------------------------------------===//
// Flatten
//===----------------------------------------------------------------------===//
Flatten::Flatten()
  : ComputeOperator("Flatten", ID),
    m_Axis(1) {
}

Flatten::Flatten(const IntAttr& pAxis)
  : ComputeOperator("Flatten", ID),
    m_Axis(pAxis) {
}

Flatten::Flatten(const Flatten& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axis(pCopy.getAxis()) {
}

void Flatten::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxis() << ">";
}

bool Flatten::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
