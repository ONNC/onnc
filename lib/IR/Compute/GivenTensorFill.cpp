//===- GivenTensorFill.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/GivenTensorFill.h>

using namespace onnc;

char GivenTensorFill::ID = 0;

//===----------------------------------------------------------------------===//
// GivenTensorFill
//===----------------------------------------------------------------------===//
GivenTensorFill::GivenTensorFill()
  : ComputeOperator("GivenTensorFill", ID),
    m_ExtraShape(),
    m_InputAsShape(),
    m_Shape(),
    m_Values() {
}

GivenTensorFill::GivenTensorFill(const IntsAttr& pExtraShape,
                                 const IntAttr& pInputAsShape,
                                 const IntsAttr& pShape,
                                 const FloatsAttr& pValues)
  : ComputeOperator("GivenTensorFill", ID),
    m_ExtraShape(pExtraShape),
    m_InputAsShape(pInputAsShape),
    m_Shape(pShape),
    m_Values(pValues) {
}

GivenTensorFill::GivenTensorFill(const GivenTensorFill& pCopy)
  : ComputeOperator(pCopy) /* shallow copy */,
    m_ExtraShape(pCopy.getExtraShape()),
    m_InputAsShape(pCopy.getInputAsShape()),
    m_Shape(pCopy.getShape()),
    m_Values(pCopy.getValues()) {
}

void GivenTensorFill::print(std::ostream& pOS) const
{
  pOS << name() << "< " << getExtraShape() << ", " << getInputAsShape() << ", " << getShape() << ", " << getValues() << ">";
}

bool GivenTensorFill::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
