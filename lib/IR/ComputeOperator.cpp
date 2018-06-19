//===- ComputeOperator.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/ComputeOperator.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ComputeOperator
//===----------------------------------------------------------------------===//
void ComputeOperator::connect(const GraphOperator& pGraphOperator)
{
  m_GraphOperators.push_back(&pGraphOperator);
}
