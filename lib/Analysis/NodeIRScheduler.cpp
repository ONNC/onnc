//===- NodeIRScheduler.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/NodeIRScheduler.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Core/AnalysisUsage.h>
#include <onnc/Core/InitializePasses.h>
#include <onnc/IR/Dump.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Target/TargetTransformInfo.h>
#include <iomanip> // for setw

using namespace onnc;

//===----------------------------------------------------------------------===//
// Extension IR for onnx
//===----------------------------------------------------------------------===//
static const onnx::Symbol g_LoadKind("Load");
static const onnx::Symbol g_StoreKind("Store");

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
static bool HasInsertedLoadStoreNode(onnx::Graph &pGraph)
{
  return pGraph.nodes().rbegin()->kind() == g_StoreKind;
}

static void InsertLoadStoreNode(onnx::Graph &pGraph)
{
  for (onnx::Value* v : pGraph.inputs()) {
    onnx::Node* first = nullptr;
    for(auto u : v->uses()) {
      if (!first) {
        first = u.user;
        continue;
      }

      if (!first->isBefore(u.user))
        first = u.user;
    }

    // Create load node and insert before the first use node.
    onnx::Node* loadN = pGraph.create(g_LoadKind);
    loadN->insertBefore(first);
    loadN->output()->copyMetadata(v);
    v->replaceAllUsesWith(loadN->output());
  }

  for (onnx::Value* v : pGraph.outputs()) {
    onnx::Node* last = nullptr;
    for(auto u : v->uses()) {
      if (!last) {
        last = u.user;
        continue;
      }

      if (last->isBefore(u.user))
        last = u.user;
    }

    // Create store node and insert before the last use node.
    onnx::Node* storeN = pGraph.create(g_StoreKind, {v}/*, 0*/);
    storeN->output()->copyMetadata(v);
    storeN->insertBefore(last);
  }
}

//===----------------------------------------------------------------------===//
// NodeIRScheduler
//===----------------------------------------------------------------------===//
NodeIRScheduler::NodeIRScheduler(DLATargetBackend* pDLATB)
  : ModulePass(ID), m_DLATB(pDLATB) {
}

NodeIRScheduler::~NodeIRScheduler()
{
}

Pass::ReturnType NodeIRScheduler::runOnModule(Module& pModule)
{
  if (!m_DLATB) {
    errs() << "No backend infomation that is needed for memory allcation.\n";
    return kPassFailure;
  }

  onnx::Graph &graph = *pModule.getGraph();
  if (!HasInsertedLoadStoreNode(graph));
    InsertLoadStoreNode(graph);

  for (onnx::Node *n : graph.nodes()) {
    if (n->kind() == onnx::kUndefined)
      continue;

    int c = m_DLATB->getTTI()
                   ->getOperatorCost(n, TargetTransformInfo::kCycleCount);

    outs() << std::setw(12) << std::left << c;
    PrintNode(outs(), *n);
  }

  return kModuleNoChanged;
}

void NodeIRScheduler::getAnalysisUsage(AnalysisUsage& pUsage) const
{
  pUsage.addRequiredID(UpdateGraphOutputSize::ID);
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char NodeIRScheduler::ID = 0;

namespace onnc
{
  INITIALIZE_DLA_PASS(NodeIRScheduler, "NodeIRScheduler")
}

NodeIRScheduler *onnc::CreateNodeIRSchedulerPass(DLATargetBackend *pDLATB)
{
  return new NodeIRScheduler(pDLATB);
}
