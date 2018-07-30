//===- Softmax.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Softmax.h>

using namespace onnc;

char Softmax::ID = 0;

//===----------------------------------------------------------------------===//
// Softmax
//===----------------------------------------------------------------------===//
Softmax::Softmax()
  : ComputeOperator("Softmax", ID),
    m_Axis(1) {
}

Softmax::Softmax(const IntAttr& pAxis)
  : ComputeOperator("Softmax", ID),
    m_Axis(pAxis) {
}

Softmax::Softmax(const Softmax& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Axis(pCopy.getAxis()) {
}

void Softmax::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getAxis() << ">";
}

bool Softmax::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
