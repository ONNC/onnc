//===- Value.cpp ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Value.h>
#include <onnc/IR/ComputeOperator.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Value
//===----------------------------------------------------------------------===//
void Value::adapt(xTensor& pAdaptee)
{
  m_pAdaptee = &pAdaptee;
  m_Name = pAdaptee.name();
}

void Value::replaceAllUsesWith(Value& pValue)
{
  UseList::iterator use;
  std::vector<onnc::Use> usedValues(m_UseList.begin(), m_UseList.end());
  for (auto use : usedValues) {
    // replace the definition's input value by pValue
    use.getUser()->replaceInput(use.getOperandNo(), pValue);
  }
}
