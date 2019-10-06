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
    m_GraphOperators(pCopy.m_GraphOperators) {}

void ComputeOperator::removeAllInputs() {
  for (Value *oldV : m_Inputs) {
    // erase this from intput Values' UseList
    Value::UseList &uList = oldV->getUses();
    unsigned oldUseId = 0;

    for (; oldUseId < uList.size(); ++oldUseId) {
      if (uList[oldUseId].getUser() == this) {
        break;
      }
    }

    if (!(oldUseId < uList.size())) {
      continue;
    }

    uList.erase(std::next(begin(uList), oldUseId));
  }
  m_Inputs.clear();
}

void ComputeOperator::removeAllOutputs() {
  for (Value *oldV : m_Outputs)
    oldV->clearDefine();
  m_Outputs.clear();
}

void ComputeOperator::connect(const GraphOperator& pGraphOperator)
{
  m_GraphOperators.push_back(&pGraphOperator);
}

void ComputeOperator::insertInput(unsigned int pIdx, onnc::Value &pValue) {
  if (pIdx > m_Inputs.size())
    fatal(input_out_of_range) << pIdx << name() << (uint32_t)m_Inputs.size();

  m_Inputs.insert(std::next(begin(m_Inputs), pIdx), &pValue);
  pValue.getUses().emplace_back(*this, pIdx);
}

void ComputeOperator::replaceInput(unsigned int pIdx, onnc::Value& pValue)
{
  if (pIdx >= m_Inputs.size())
    fatal(input_out_of_range) << pIdx << name() << (uint32_t)m_Inputs.size();

  onnc::Value* oldV = m_Inputs[pIdx];
  if (oldV == &pValue)
    return;

  // Remove this ComputeOperator from oldV's use_list
  unsigned oldUseId = 0;
  for (; oldUseId < oldV->getUses().size(); ++oldUseId)
    if (oldV->getUses()[oldUseId].getUser() == this)
      break;

  assert(oldUseId < oldV->getUses().size() &&
         "onnc::Value has no specified user.");

  oldV->getUses().erase(std::next(begin(oldV->getUses()), oldUseId));

  // Replace with new value and update use list.
  m_Inputs[pIdx] = &pValue;
  pValue.getUses().emplace_back(*this, pIdx);
}

void ComputeOperator::replaceOutput(unsigned int pIdx, onnc::Value &pValue)
{
  // FIXEME changed input_out_of_range
  if (pIdx >= m_Outputs.size())
    fatal(input_out_of_range) << pIdx << name() << (uint32_t)m_Outputs.size();
  m_Outputs[pIdx]->clearDefine();
  m_Outputs[pIdx]->replaceAllUsesWith(pValue);
  m_Outputs[pIdx] = &pValue;

  if (!pValue.getDefine()) {
    pValue.setDefine(this, pIdx);
  }
}

void ComputeOperator::removeInput(unsigned int pIdx) {
  if (pIdx >= m_Inputs.size())
    fatal(input_out_of_range) << pIdx << name() << (uint32_t)m_Inputs.size();

  Value *oldV = m_Inputs[pIdx];
  Value::UseList &uList = oldV->getUses();
  unsigned oldUseId = 0;

  for (; oldUseId < uList.size(); ++oldUseId)
    if (uList[oldUseId].getUser() == this)
      break;

  assert(oldUseId < uList.size() && "onnc::Value has no specified user.");

  uList.erase(std::next(begin(uList), oldUseId));
  m_Inputs.erase(std::next(begin(m_Inputs), pIdx));
}

void ComputeOperator::print(std::ostream& pOS) const
{
  { // Print Outputs
    bool first = true;
    for (const Value *v : m_Outputs) {
      if (!first)
        pOS << ", ";
      v->print(pOS);
      first = false;
    }
  }
  // Print operator name & attributes
  pOS << " = " << name();
  printAttributes(pOS);
  { // Print Inputs
    bool first = true;
    pOS << '(';
    for (const Value *v : m_Inputs) {
      if (!first)
        pOS << ", ";
      v->print(pOS);
      first = false;
    }
    pOS << ')';
  }
}

void ComputeOperator::print(json::Value& pJSON) const
{
  // TODO: implement it!
}
