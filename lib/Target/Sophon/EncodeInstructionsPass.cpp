//===- EncodeInstructionsPass.cpp -----------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "EncodeInstructionsPass.h"
#include <onnc/IR/ComputeOperator.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// BuildMemOpnd
//===----------------------------------------------------------------------===//
EncodeInstructions::EncodeInstructions(ComputeVisitor *pInstVisitor)
    : m_InstEmitVisitors(pInstVisitor)
{
}

Pass::ReturnType
EncodeInstructions::runOnComputeGraph(::onnc::ComputeGraph &pCG)
{
  ComputeGraph::iterator nodeIt, nEnd = pCG.end();
  for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
    const onnc::ComputeOperator *node = nodeIt;
    beforeEmit(node);
    node->accept(*m_InstEmitVisitors);
  }
  return Pass::kModuleNoChanged;
}

ModulePass *onnc::CreateEncodeInstructionsPass(ComputeVisitor *pInstVisitor)
{
  return new EncodeInstructions(pInstVisitor);
}
