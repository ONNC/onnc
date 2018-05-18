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
#include <onnc/Support/IOStream.h>

using namespace onnc;

static void PrintNode(OStream &pOS,::onnx::Node& pNode)
{
  pOS << "  ";

  // print output.
  for (int i = 0; i < pNode.outputs().size(); ++i) {
    if (i != 0) { pOS << ", "; }

   ::onnx::Value* v = pNode.outputs()[i];
    pOS << '%' << v->uniqueName();
  }
  pOS << " = " << pNode.kind().toString();
  pOS << '(';
  for (int i = 0; i < pNode.inputs().size(); ++i) {
    if (i != 0) { pOS << ", "; }
   ::onnx::Value* v = pNode.inputs()[i];
    pOS << '%' << v->uniqueName();
  }
  pOS << ")\n";
}

static void PrintGraph(OStream &pOS,::onnx::Graph& pGraph)
{
  pOS << "graph " << pGraph.name() << " (..) {" << "\n";
  for (::onnx::Node *n : pGraph.nodes()) {
    if (n->kind() ==::onnx::kUndefined)
      continue;

    PrintNode(pOS, *n);
  }

  pOS << "  return ";
  for (int i = 0; i < pGraph.outputs().size(); i++) {
    if (i != 0) { pOS << ", "; }

   ::onnx::Value* v = pGraph.outputs()[i];
    pOS << "%" << v->uniqueName();
  }
  pOS << "\n";

}

void onnc::DumpGraph(::onnx::Graph& pGraph)
{
  PrintGraph(errs(), pGraph);
}

void onnc::DumpGraph(Module &pModule)
{
  PrintGraph(errs(), *pModule.getGraph());
}
