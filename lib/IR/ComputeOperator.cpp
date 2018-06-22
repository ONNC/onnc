//===- ComputeOperator.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/ComputeOperator.h>
#include <onnc/Diagnostic/MsgHandling.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ComputeOperator
//===----------------------------------------------------------------------===//
void ComputeOperator::connect(const GraphOperator& pGraphOperator)
{
  m_GraphOperators.push_back(&pGraphOperator);
}

void ComputeOperator::replaceInput(unsigned int pIdx, onnc::Value& pValue)
{
  if (pIdx >= m_Inputs.size())
    fatal(input_out_of_range) << pIdx << name() << (uint32_t)m_Inputs.size();
  m_Inputs[pIdx] = &pValue;
}
