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

char EncodeInstructions::ID = 0;

//===----------------------------------------------------------------------===//
// BuildMemOpnd
//===----------------------------------------------------------------------===//
EncodeInstructions::EncodeInstructions(ComputeVisitor *pInstVisitor)
    : ModulePass(ID), m_InstEmitVisitors(pInstVisitor)
{
}

Pass::ReturnType EncodeInstructions::runOnModule(::onnc::Module &pModule)
{
  unsigned result = Pass::kModuleNoChanged;
  Module::cg_iterator cg, cgEnd = pModule.cgEnd();
  for (cg = pModule.cgBegin(); cg != cgEnd; ++cg)
    result |= runOnComputeGraph(*cg->value());
  return result;
}

Pass::ReturnType
EncodeInstructions::runOnComputeGraph(::onnc::ComputeGraph &pCG)
{
  ComputeGraph::iterator nodeIt, nEnd = pCG.end();
  for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
    const onnc::ComputeOperator *node = nodeIt;
    node->accept(*m_InstEmitVisitors);
  }
  return Pass::kModuleNoChanged;
}

ModulePass *onnc::CreateEncodeInstructionsPass(ComputeVisitor *pInstVisitor)
{
  return new EncodeInstructions(pInstVisitor);
}