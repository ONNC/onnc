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
#include <onnx/onnx_pb.h>

using namespace onnc;

static void PrintValue(OStream &pOS, const ::onnx::Value *pValue)
{
  pOS << TensorProto_DataType_Name(pValue->elemType()) << " tensor ";
  // print dimension
  auto sizes = pValue->sizes();
  pOS << "<";
  for (int i = 0; i < sizes.size(); ++i) {
    if (i != 0) {
      pOS << ", ";
    }
    if (sizes[i].is_int) {
      pOS << sizes[i].dim;
      continue;
    }
    pOS << sizes[i].param;
  }
  pOS << "> ";
  pOS << '%' << pValue->uniqueName();
}

static void PrtinAttribute(OStream &pOS, const ::onnx::Node &pNode)
{
  auto attrNames = pNode.attributeNames();
  if (attrNames.size() != 0)
    pOS << " <";
  for (int i = 0; i < attrNames.size(); ++i) {
    if (i != 0) {
      pOS << ", ";
    }
    ::onnx::Symbol name = attrNames[i];
    pOS << name.toString() << ":";
    switch (pNode.kindOf(name)) {
    case ::onnx::AttributeKind::f:
      pOS << "FLOAT " << static_cast<float>(pNode.f(name));
      break;
    case ::onnx::AttributeKind::fs: {
      pOS << "FLOATS [";
      auto fs = pNode.fs(name);
      for (int i = 0; i < fs.size(); ++i) {
        if (i != 0)
          pOS << ",";
        pOS << static_cast<float>(fs[i]) << ", ";
      }
      pOS << "]";
      break;
    }
    case ::onnx::AttributeKind::i:
      pOS << "INT " << pNode.i(name);
      break;
    case ::onnx::AttributeKind::is: {
      pOS << "INTS [";
      auto is = pNode.is(name);
      for (int i = 0; i < is.size(); ++i) {
        if (i != 0)
          pOS << ",";
        pOS << is[i];
      }
      pOS << "]";
      break;
    }
    case ::onnx::AttributeKind::s:
      pOS << "STRING ";
      pOS << pNode.s(name);
      break;
    case ::onnx::AttributeKind::ss: {
      pOS << "STRINGS [";
      auto ss = pNode.ss(name);
      for (int i = 0; i < ss.size(); ++i) {
        if (i != 0)
          pOS << ",";
        pOS << ss[i] << ",";
      }
      pOS << "]";
      break;
    }
    case ::onnx::AttributeKind::t: {
      pOS << "TENSOR " << pNode.t(name).name();
    } break;
    case ::onnx::AttributeKind::ts: {
      pOS << "TENSORS [";
      auto ts = pNode.ts(name);
      for (int i = 0; i < ts.size(); ++i) {
        if (i != 0)
          pOS << ",";
        pOS << ts[i].name() << ",";
      }
      pOS << "]";
      break;
    }
    case ::onnx::AttributeKind::g: {
      pOS << "GRAPH " << pNode.g(name).get()->name();
    } break;
    case ::onnx::AttributeKind::gs: {
      pOS << "GRAPHS [";
      auto gs = pNode.gs(name);
      for (int i = 0; i < gs.size(); ++i) {
        if (i != 0)
          pOS << ",";
        pOS << gs[i].get()->name() << ",";
      }
      pOS << "]";
      break;
    }
    }
  }
  if (attrNames.size() != 0)
    pOS << "> ";
}

void onnc::PrintNode(OStream &pOS, ::onnx::Node &pNode)
{
  pOS << "  ";

  // print outputs.
  for (int i = 0; i < pNode.outputs().size(); ++i) {
    if (i != 0) {
      pOS << ", ";
    }
    ::onnx::Value *v = pNode.outputs()[i];
    PrintValue(pOS, v);
  }
  pOS << " = " << pNode.kind().toString();

  // print attributes.
  PrtinAttribute(pOS, pNode);

  // print inputs.
  pOS << '(';
  for (int i = 0; i < pNode.inputs().size(); ++i) {
    if (i != 0) {
      pOS << ", ";
    }
    ::onnx::Value *v = pNode.inputs()[i];
    PrintValue(pOS, v);
  }
  pOS << ")\n";
}

void onnc::PrintGraph(OStream &pOS, ::onnx::Graph &pGraph)
{
  pOS << "graph " << pGraph.name() << " (";
  std::unordered_set<std::string> initializerNames(
      pGraph.initializer_names().begin(), pGraph.initializer_names().end());

  // dump graph input
  for (int i = 0; i < pGraph.inputs().size(); ++i) {
    auto *v = pGraph.inputs()[i];
    if (0 != initializerNames.count(v->uniqueName())) {
      continue;
    }
    if (i != 0) {
      pOS << ", ";
    }
    PrintValue(pOS, v);
  }
  pOS << ")\n";

  for (::onnx::Node *n : pGraph.nodes()) {
    if (n->kind() == ::onnx::kUndefined)
      continue;

    PrintNode(pOS, *n);
  }

  pOS << "  return ";
  for (int i = 0; i < pGraph.outputs().size(); i++) {
    if (i != 0) {
      pOS << ", ";
    }

    ::onnx::Value *v = pGraph.outputs()[i];
    PrintValue(pOS, v);
  }
  pOS << "\n";
}

void onnc::DumpGraph(::onnx::Graph &pGraph) { PrintGraph(outs(), pGraph); }

void onnc::DumpGraph(Module &pModule)
{
  PrintGraph(outs(), *pModule.getGraphIR());
}

void onnc::DumpModule(Module &pModule)
{
  // dump model info
  outs() << "ir_version:" << pModule.getOnnxInfo().getIRVersion() << "\n";
  outs() << "producer_name:" << pModule.getOnnxInfo().getProducerName() << "\n";
  outs() << "producer_version:" << pModule.getOnnxInfo().getProducerVersion() << "\n";
  outs() << "domain:" << pModule.getOnnxInfo().getDomain() << "\n";
  outs() << "model_version:" << pModule.getOnnxInfo().getModelVersion() << "\n";
  outs() << "doc_string:" << pModule.getOnnxInfo().getDocString() << "\n";

  DumpGraph(pModule);

  // dump opset_imports
  for (auto setId : pModule.getSetId()) {
    outs() << "opset_import.domain:" << setId.first << "\n";
    outs() << "opset_import.version:" << setId.second << "\n";
  }

  // dump metadata_props
  for (auto metaData : pModule.getMetaData()) {
    outs() << "metadata_props.key:" << metaData.first << "\n";
    outs() << "metadata_props.value:" << metaData.second << "\n";
  }
}
