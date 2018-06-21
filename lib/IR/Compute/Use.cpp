//===- Use.cpp ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Use.h>
#include <onnc/IR/Compute/Value.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/Diagnostic/MsgHandling.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Use
//===----------------------------------------------------------------------===//
Use::Use(ComputeOperator& pOperator, unsigned int pIdx)
  : m_pUser(&pOperator), m_OperandNo(pIdx), m_pValue(nullptr) {
  if (pIdx >= m_pUser->getNumOfInputs()) {
    fatal(use_out_of_range) << pIdx
                            << pOperator.name()
                            << pOperator.getNumOfInputs();
  }
  m_pValue = m_pUser->getInput(pIdx);
}
