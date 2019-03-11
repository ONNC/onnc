//===- GenWeightPass.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "GenWeightPass"
#include "GenWeightPass.h"
#include "FillWeightVisitor.h"
#include <onnc/Support/Debug.h>
#include <onnc/Target/Sophon/io.hpp>

using namespace onnc;

//===----------------------------------------------------------------------===//
// GenWeightPass
//===----------------------------------------------------------------------===//
BM188X::GenWeightPass::GenWeightPass(TGBackend* pBackend, const Path &pOutFile)
    : m_pBackend(pBackend), m_OutFile(pOutFile), m_Weight()
{
}

Pass::ReturnType BM188X::GenWeightPass::runOnModule(Module &pModule)
{
  fillWeight(pModule);
  if (!m_Weight.empty()) {
    std::string filename = m_OutFile.native() + ".weight.bin";
    bmnet::WriteInt8DataToBinaryFile(&m_Weight, filename);
  }
  return kModuleNoChanged;
}

void BM188X::GenWeightPass::fillWeight(const Module& pModule)
{
  // initialize weight's size
  size_t weight_size = 0;
  const Module::ValueList& value_list = pModule.getValueList();
  const auto vEnd = value_list.end();
  std::unordered_set<const ComputeMemOperand*> allocatedMemOpnd;
  for (auto value = value_list.begin(); value != vEnd; ++value) {
    const ComputeMemOperand* mem_opnd =
        backend()->getMemOpndByValue(value->value());
    // FIXME some values does not have the ComputeMemOperand
    if (mem_opnd == nullptr)
      continue;
    // ignore duplicate ComputeMemOperand
    if (allocatedMemOpnd.find(mem_opnd) != allocatedMemOpnd.end()) {
      continue;
    }
    weight_size += mem_opnd->length();
    allocatedMemOpnd.insert(mem_opnd);
  }

  // reserve space.
  m_Weight.reserve(weight_size);

  DEBUG(dbgs() << "GenWeightPass: reserve space is " << weight_size << "\n");

  FillWeightVisitor visitor(m_Weight);
  Module::const_cg_iterator cg, cEnd = pModule.cgEnd();
  // ps. currently traverse memory order need to sync with LinearScanAllocPass
  for (cg = pModule.cgBegin(); cg != cEnd; ++cg) {
    const ComputeGraph* graph = cg->value();
    ComputeGraph::const_iterator node, nEnd = graph->end();
    for (node = graph->begin(); node != nEnd; ++node) {
      node->accept(visitor);
    } // for each node
  } // for each compute graph

  DEBUG(dbgs() << "GenWeightPass: final weight size is " << m_Weight.size()
               << "\n");
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
ModulePass*
BM188X::CreateGenWeightPass(TGBackend* pBackend, const Path& pOutFile)
{
  return new GenWeightPass(pBackend, pOutFile);
}
