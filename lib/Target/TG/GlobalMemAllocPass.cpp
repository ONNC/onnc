//===- GlobalMemAllocPass.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "GlobalMemAllocPass.h"
#include "BM188x/BM188xVisitor.h"
#include "TG.h"
#include "TGBackend.h"
#include <onnc/ADT/Color.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/Support/Debug.h>
#include <onnc/Support/IOStream.h>
#include <onnx/common/ir.h>

#define DEBUG_TYPE "tg_mem_alloc"

using namespace onnc;
using namespace onnc::BM188X;

char GlobalMemAlloc::ID = 0;

static void CreateMemOperands(onnc::ComputeGraph &pCG, ComputeOperator &pNode,
                              ComputeOperand::Residence pResd)
{
  unsigned int out_size = pNode.getNumOfOutputs();
  for (unsigned int i = 0; i < out_size; ++i) {
    onnc::Value* value = pNode.getOutput(i);
    onnc::Value::UseList& use_list = value->getUses();
    onnc::Value::UseList::iterator use, uEnd = use_list.end();
    for (use = use_list.begin(); use != uEnd; ++use)
      pCG.addOperand<ComputeMemOperand>(pNode, *use->getUser(),
                                        *value, pResd);
  }
}

// For handle special cases.
class GlobalMemAllocVisitor : public BM188X::BM188xVisitor
{
public:
  GlobalMemAllocVisitor(onnc::ComputeGraph &pCG)
    : m_Processed(false), m_CG(pCG) {
  }

  void visit(Initializer &pInitializer) override
  {
    m_Processed = true;
    CreateMemOperands(m_CG, pInitializer, ComputeOperand::kWeightResidence);
  }

  bool isProcessed() const { return m_Processed; }

  void reset() { m_Processed = false; }

  // TLLoad TLStore

private:
  bool m_Processed;
  onnc::ComputeGraph &m_CG;
};

//===----------------------------------------------------------------------===//
// GlobalMemAlloc
//===----------------------------------------------------------------------===//
GlobalMemAlloc::GlobalMemAlloc(TGBackend *pTarget)
    : ModulePass(ID), m_pTarget(pTarget)
{
}

Pass::ReturnType GlobalMemAlloc::runOnModule(::onnc::Module &pModule)
{
  Module::cg_iterator cg, cgEnd = pModule.cgEnd();
  for (cg = pModule.cgBegin(); cg != cgEnd; ++cg)
    runOnComputeGraph(*cg->value());

  return Pass::kModuleNoChanged;
}

void GlobalMemAlloc::runOnComputeGraph(onnc::ComputeGraph &pCG)
{
  GlobalMemAllocVisitor gmallocVisitor(pCG);
  ComputeGraph::iterator nodeIt, nEnd = pCG.end();
  for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
    // process special cases.
    gmallocVisitor.reset();
    nodeIt->accept(gmallocVisitor);
    if (gmallocVisitor.isProcessed())
      continue;

    // process general case.
    CreateMemOperands(pCG, *nodeIt, ComputeOperand::kOutputResidence);
  }
}

ModulePass *onnc::CreateGlobalMemAllocPass(TGBackend *pTarget)
{
  return new GlobalMemAlloc(pTarget);
}