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
ComputeOperator::ComputeOperator(StringRef pName, char& pID)
  : onnc::Define(pName),
    m_OpCode(0), m_ID(&pID), m_Inputs(), m_Outputs(), m_GraphOperators() {
}

ComputeOperator::ComputeOperator(const ComputeOperator& pCopy)
  : onnc::Define(pCopy),
    onnc::DigraphNode<ComputeOperator, ComputeOperand>(), // no linkages
    m_OpCode(pCopy.getOpCode()),
    m_ID(pCopy.getID()),
    m_Inputs(pCopy.m_Inputs),
    m_Outputs(pCopy.m_Outputs),
    m_GraphOperators(pCopy.m_GraphOperators) {
}

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
