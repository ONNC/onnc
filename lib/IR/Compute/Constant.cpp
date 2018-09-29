//===- Constant.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Constant.h>

using namespace onnc;

char Constant::ID = 0;

//===----------------------------------------------------------------------===//
// Constant
//===----------------------------------------------------------------------===//
Constant::Constant(const TensorAttr& pValue)
  : ComputeOperator("Constant", ID),
    m_Value(pValue) {
}



Constant::Constant(const Constant& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_Value(pCopy.getValue()) {
}

void Constant::printAttributes(std::ostream& pOS) const
{
  pOS << '<' << "value: " << getValue()<< '>';
}

bool Constant::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
