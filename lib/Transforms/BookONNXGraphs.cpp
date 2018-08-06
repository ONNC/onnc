//===- BookONNXGraphs.cpp -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/IRBuilder.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Transforms/BookONNXGraphs.h>
#include <vector>

using namespace onnc;

char BookONNXGraphs::ID = 0;

//===----------------------------------------------------------------------===//
// BookONNXGraphs
//===----------------------------------------------------------------------===//
BookONNXGraphs::BookONNXGraphs()
  : ModulePass(ID) {
}

BookONNXGraphs::~BookONNXGraphs()
{
}

Pass::ReturnType BookONNXGraphs::runOnModule(::onnc::Module &pModule)
{
  IRBuilder builder(pModule);

  std::vector<xGraph*> worklist;
  worklist.push_back(pModule.getRootTensorGraph());
  int graphCnt = 0;

  while (!worklist.empty()) {
    xGraph* graph = worklist.back();
    worklist.pop_back();

    // Don't go further if the graph has been added.
    if (pModule.recordSubgraph(*graph))
      continue;

    ++graphCnt;
    // Create compute graph.
    if (graph->has_name())
      builder.CreateComputeGraph(graph->name());
    else
      builder.CreateComputeGraph("onnc-graph-" + std::to_string(graphCnt));

    for (xNode *n : graph->nodes()) {
      if (n->kind() == xValueType::kUndefined)
        continue;

      if (n->hasAttribute(xBuiltinSymbol::kSubgraph)) {
        worklist.push_back(&*n->g(xBuiltinSymbol::kSubgraph));
      }
    } // end of for each onnx node
  } // end of stack empty checking

  return Pass::kModuleNoChanged;
}

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
INITIALIZE_PASS(BookONNXGraphs, "BookONNXGraphs");

ModulePass *onnc::CreateBookONNXGraphs()
{
  return new BookONNXGraphs();
}
