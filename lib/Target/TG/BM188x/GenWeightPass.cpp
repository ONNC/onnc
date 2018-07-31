//===- GenWeightPass.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "GenWeightPass.h"
#include "FillWeightVisitor.h"
#include <onnc/Target/TG/io.hpp>

using namespace onnc;

char BM188X::GenWeightPass::ID = 0;

//===----------------------------------------------------------------------===//
// GenWeightPass
//===----------------------------------------------------------------------===//
BM188X::GenWeightPass::GenWeightPass(const Path &pOutFile)
    : ModulePass(ID), m_OutFile(pOutFile), m_Weight(), m_DoneOpndSet()
{
}

Pass::ReturnType BM188X::GenWeightPass::runOnModule(Module &pModule)
{
  fillWeight(pModule);
  if (!m_Weight.empty())
    bmnet::WriteInt8DataToBinaryFile(&m_Weight, m_OutFile.c_str());
  return kModuleNoChanged;
}

void BM188X::GenWeightPass::fillWeight(const Module& pModule)
{
  // initialize weight's size
  size_t weight_size = 0;
  const Module::ComputeOperandList& opnd_list = pModule.getComputeOperands();
  Module::ComputeOperandList::const_iterator opnd, oEnd = opnd_list.end();
  for (opnd = opnd_list.begin(); opnd != oEnd; ++opnd) {
    const ComputeMemOperand* mem_opnd =
          dyn_cast<const ComputeMemOperand>(*opnd);
    if (nullptr != mem_opnd && mem_opnd->isWeight()) {
      weight_size += mem_opnd->length();
    }
  }

  // reserve space.
  m_Weight.reserve(weight_size);

  FillWeightVisitor visitor(m_Weight);
  Module::const_cg_iterator cg, cEnd = pModule.cgEnd();
  for (cg = pModule.cgBegin(); cg != cEnd; ++cg) {
    const ComputeGraph* graph = cg->value();
    ComputeGraph::const_iterator node, nEnd = graph->end();
    for (node = graph->begin(); node != nEnd; ++node) {
      node->accept(visitor);
/**
      if (isa<BM188X::Conv>(*node)) {
      }
      else if (isa<BM188X::SlicedConv>(*node)) {
      }
      else if (isa<onnc::Initializer>(*node)) {
        onnc::Initializer* init = dyn_cast<onnc::Initializer>(*node);
        if (onnc::Value::kInt8 == init->getTensor<onnc::Value>()->kind()) {
        }
        else {
        }
      }
**/
    } // for each node
  } // for each compute graph
}

bool BM188X::GenWeightPass::isWritten(const ComputeMemOperand* pOpnd) const
{
  return (m_DoneOpndSet.end() != m_DoneOpndSet.find(pOpnd));
}

void BM188X::GenWeightPass::setWritten(const ComputeMemOperand* pOpnd)
{
  m_DoneOpndSet.insert(pOpnd);
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
ModulePass*
BM188X::CreateGenWeightPass(const Path& pOutFile)
{
  return new GenWeightPass(pOutFile);
}
