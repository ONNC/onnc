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
BM188X::GenWeightPass::GenWeightPass(TGBackend* pBackend, const Path &pOutFile)
    : ModulePass(ID), m_pBackend(pBackend), m_OutFile(pOutFile), m_Weight()
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
  const Module::ValueList& value_list = pModule.getValueList();
  Module::ValueList::const_iterator value, vEnd = value_list.end();
  for (value = value_list.begin(); value != vEnd; ++value) {
    const ComputeMemOperand* mem_opnd =
        backend()->getMemOpndByValue(value->value());
    weight_size += mem_opnd->length();
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
    } // for each node
  } // for each compute graph
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
ModulePass*
BM188X::CreateGenWeightPass(TGBackend* pBackend, const Path& pOutFile)
{
  return new GenWeightPass(pBackend, pOutFile);
}
