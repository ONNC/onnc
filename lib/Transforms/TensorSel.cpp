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
#include <onnc/Support/String.h>
#include <onnc/Config/ONNX.h>
#include <onnc/IR/Dump.h>

#include <tuple>
#include <stack>
#include <string>

using namespace onnc;

//===----------------------------------------------------------------------===//
// TensorSel
//===----------------------------------------------------------------------===//
namespace onnc {
namespace detail {
  std::string getDisplayName(const xNode& node)
  {
    return to_string(node.kind().toString(), "(name=\"", node.name(), "\")");
  }
} // namespace detail
} // namespace onnc

TensorSel::TensorSel(const TargetBackend* pBackend)
  : m_pBackend(pBackend), m_LowerRegistry() {
  if (nullptr != m_pBackend) {
    m_pBackend->RegisterLowers(m_LowerRegistry);
  }
}

TensorSel::~TensorSel()
{
  m_LowerRegistry.clear();
}

Pass::ReturnType TensorSel::runOnGraphs(xGraph& pTG, ComputeGraph& pCG)
{
  xGraphNodeListIterator tg_node, tg_end = pTG.end();
  for (tg_node = pTG.begin(); tg_node != tg_end; ++tg_node) {
    // lower creates corresponding compute operator and values
    Lower* lower = m_LowerRegistry.lookup(**tg_node);
    if (nullptr == lower) {
      fatal(no_corre_lower) << detail::getDisplayName(**tg_node);
      return Pass::kPassFailure;
    }
    if (nullptr == lower->activate(pCG, **tg_node)) {
      errs() << "Failed to lowering: ";
      if (tg_node->has_name())
        errs() << tg_node->name() << ", ";
      errs() << "node type = " << tg_node->kind().toString() << "\n";
    }
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
