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
#include <tuple>
#include <stack>

using namespace onnc;

char TensorSel::ID = 0;

//===----------------------------------------------------------------------===//
// TensorSel
//===----------------------------------------------------------------------===//
TensorSel::TensorSel(const TargetBackend* pBackend)
  : ModulePass(ID), m_pBackend(pBackend), m_LowerRegistry() {
  if (nullptr != m_pBackend) {
    m_pBackend->RegisterLowers(m_LowerRegistry);
  }
}

TensorSel::~TensorSel()
{
  m_LowerRegistry.clear();
}

Pass::ReturnType TensorSel::runOnModule(::onnc::Module &pModule)
{
  typedef std::tuple<::onnx::Graph*, ComputeGraph*,
                     ::onnx::graph_node_list_iterator> tag;
  std::stack<tag> worklist;

  ::onnx::Graph* topOnnxG = pModule.getRootTensorGraph();
  ComputeGraph* topCG = pModule.cgBegin()->value();

  worklist.push(tag{topOnnxG, topCG, topOnnxG->begin()});

  IRBuilder builder(pModule);

  // iterative based dfs lowering onnx graph.
  while (!worklist.empty()) {
    ::onnx::Graph* onnxG;
    ComputeGraph* computeG;
    ::onnx::graph_node_list_iterator onnxNIter;

    std::tie(onnxG, computeG, onnxNIter) = worklist.top();
    worklist.pop();

    while (onnxNIter != onnxG->end()) {
      ::onnx::Node* onnxN = *onnxNIter;
      ++onnxNIter;
      if (onnxN->kind() == ::onnx::kUndefined)
        continue;

      // create new compute graph if this is subgraph node.
      if (onnxN->hasAttribute(::onnx::kSubgraph)) {
        // push current onto stack, lower new subgraph first.
        worklist.push(tag{onnxG, computeG, onnxNIter});

        onnxG = &*onnxN->g(::onnx::kSubgraph);
        computeG = builder.CreateComputeGraph(onnxG->name());
        onnxNIter = onnxG->begin();
      }
      else {
        Lower* lower = m_LowerRegistry.lookup(*onnxN);
        if (nullptr == lower) {
          if (onnxN->has_name())
            fatal(no_corre_lower) << onnxN->name();
          else
            fatal(no_corre_lower) << onnxN->kind().toString();
          return Pass::kPassFailure;
        }

        lower->activate(*computeG, *onnxN);
      }
    } // end of while
  } // end of stack empty checking

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
