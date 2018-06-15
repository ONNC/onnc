//===- Dump.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/IR/Dump.h>
#include <onnc/IR/Module.h>

using namespace onnc;

void onnc::PrintNode(OStream &pOS, const onnx::Node& pNode)
{
  pOS << "  ";

  // print output.
  for (int i = 0; i < pNode.outputs().size(); ++i) {
    if (i != 0) { pOS << ", "; }

    const onnx::Value* v = pNode.outputs()[i];
    pOS << '%' << v->uniqueName();
  }
  pOS << " = " << pNode.kind().toString();
  pOS << '(';
  for (int i = 0; i < pNode.inputs().size(); ++i) {
    if (i != 0) { pOS << ", "; }
    const onnx::Value* v = pNode.inputs()[i];
    pOS << '%' << v->uniqueName();
  }
  pOS << ")\n";
}

void onnc::PrintGraph(OStream &pOS, const onnx::Graph& pGraph)
{
  pOS << "graph " << pGraph.name() << " (..) {" << "\n";
  for (const onnx::Node *n : pGraph.nodes()) {
    if (n->kind() == onnx::kUndefined)
      continue;

    PrintNode(pOS, *n);
  }

  pOS << "  return ";
  for (int i = 0; i < pGraph.outputs().size(); i++) {
    if (i != 0) { pOS << ", "; }

    const onnx::Value* v = pGraph.outputs()[i];
    pOS << "%" << v->uniqueName();
  }
  pOS << "\n";

}

void onnc::DumpGraph(const onnx::Graph& pGraph)
{
  PrintGraph(errs(), pGraph);
}

void onnc::DumpGraph(Module &pModule)
{
  PrintGraph(errs(), *pModule.getGraph());
}
