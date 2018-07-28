//===- InputOperator.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/Diagnostic/MsgHandling.h>

using namespace onnc;

char InputOperator::ID = 0;

//===----------------------------------------------------------------------===//
// InputOperator
//===----------------------------------------------------------------------===//
InputOperator::InputOperator()
  : ComputeOperator("InputOperator", ID),
    m_Name() {
}

InputOperator::InputOperator(const StringAttr& pName)
  : ComputeOperator("InputOperator", ID),
    m_Name(pName) {
}

InputOperator::~InputOperator()
{
}

void InputOperator::setTensor(onnc::Tensor& pTensor)
{
  if (!m_Outputs.empty()) {
    // fatal error
    return;
  }
  m_Outputs.push_back(&pTensor);
}

void InputOperator::print(std::ostream& pOS) const
{
  pOS << name();
}

bool InputOperator::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
