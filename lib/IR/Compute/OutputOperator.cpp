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

//===----------------------------------------------------------------------===//
// OutputOperator
//===----------------------------------------------------------------------===//
OutputOperator::OutputOperator()
  : ComputeOperator("OutputOperator"),
    m_Name() {
}

OutputOperator::OutputOperator(const StringAttr& pName)
  : ComputeOperator("OutputOperator"),
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
