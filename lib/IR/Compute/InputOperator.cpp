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

//===----------------------------------------------------------------------===//
// InputOperator
//===----------------------------------------------------------------------===//
InputOperator::InputOperator()
  : ComputeOperator("InputOperator"),
    m_Name() {
}

InputOperator::InputOperator(const StringAttr& pName)
  : ComputeOperator("InputOperator"),
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
