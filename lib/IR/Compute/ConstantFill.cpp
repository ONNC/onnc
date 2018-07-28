//===- ConstantFill.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/ConstantFill.h>

using namespace onnc;

char ConstantFill::ID = 0;

//===----------------------------------------------------------------------===//
// ConstantFill
//===----------------------------------------------------------------------===//
ConstantFill::ConstantFill()
  : ComputeOperator("ConstantFill", ID),
    m_Dtype(),
    m_ExtraShape(),
    m_InputAsShape(),
    m_Shape(),
    m_Value() {
}

ConstantFill::ConstantFill(const IntAttr& pDtype,
                           const IntsAttr& pExtraShape,
                           const IntAttr& pInputAsShape,
                           const IntsAttr& pShape,
                           const FloatAttr& pValue)
  : ComputeOperator("ConstantFill", ID),
    m_Dtype(pDtype),
    m_ExtraShape(pExtraShape),
    m_InputAsShape(pInputAsShape),
    m_Shape(pShape),
    m_Value(pValue) {
}

void ConstantFill::print(std::ostream& pOS) const
{
}

bool ConstantFill::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
