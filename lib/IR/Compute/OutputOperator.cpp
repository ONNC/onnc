//===- OutputOperator.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/Diagnostic/MsgHandling.h>

using namespace onnc;

char OutputOperator::ID = 0;

//===----------------------------------------------------------------------===//
// OutputOperator
//===----------------------------------------------------------------------===//
OutputOperator::OutputOperator()
  : ComputeOperator("OutputOperator", ID),
    m_Name() {
}

OutputOperator::OutputOperator(const StringAttr& pName)
  : ComputeOperator("OutputOperator", ID),
    m_Name(pName) {
}

OutputOperator::~OutputOperator()
{
}

void OutputOperator::addTensor(onnc::Tensor& pTensor)
{
  m_Inputs.push_back(&pTensor);
}

void OutputOperator::print(std::ostream& pOS) const
{
  pOS << name();
}

bool OutputOperator::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
