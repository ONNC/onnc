//===- TensorSel.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/IRBuilder.h>
#include <onnc/Transforms/TensorSel.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/Support/IOStream.h>
#include <onnc/IR/Dump.h>
#include <tuple>
#include <stack>

using namespace onnc;

char TensorSel::ID = 0;

//===----------------------------------------------------------------------===//
// TensorSel
//===----------------------------------------------------------------------===//
TensorSel::TensorSel(const TargetBackend* pBackend)
  : GraphPairPass(ID), m_pBackend(pBackend), m_LowerRegistry() {
  if (nullptr != m_pBackend) {
    m_pBackend->RegisterLowers(m_LowerRegistry);
  }
}

TensorSel::~TensorSel()
{
  m_LowerRegistry.clear();
}

Pass::ReturnType TensorSel::runOnGraphs(::onnx::Graph& pTG, ComputeGraph& pCG)
{
  ::onnx::graph_node_list_iterator tg_node, tg_end = pTG.end();
  for (tg_node = pTG.begin(); tg_node != tg_end; ++tg_node) {
    // lower creates corresponding compute operator and values
    Lower* lower = m_LowerRegistry.lookup(**tg_node);
    if (nullptr == lower) {
      if (tg_node->has_name())
        fatal(no_corre_lower) << tg_node->name();
      else
        fatal(no_corre_lower) << tg_node->kind().toString();
      return Pass::kPassFailure;
    }
    lower->activate(pCG, **tg_node);
  }
  return Pass::kModuleChanged;
}

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
INITIALIZE_TB_PASS(TensorSel, "TensorSel");

ModulePass *onnc::CreateTensorSel(const TargetBackend* pBackend)
{
  return new TensorSel(pBackend);
}
