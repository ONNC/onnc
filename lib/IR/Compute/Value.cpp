//===- Value.cpp ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Value.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Value
//===----------------------------------------------------------------------===//
void Value::adapt(::onnx::Tensor& pAdaptee)
{
  m_pAdaptee = &pAdaptee;
  m_Name = pAdaptee.name();
}

void Value::replaceAllUsesWith(Value& pValue)
{
  UseList::iterator use, uEnd = m_UseList.end();
  for (use = m_UseList.begin(); use != uEnd; ++use) {
  }
}
