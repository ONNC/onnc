//===- Module.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Module.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Config/ONNX.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member function
//===----------------------------------------------------------------------===//
inline static void
PrintAttrs(std::ostream& pOS, const xAttributes<xNode>& pAttr)
{
  std::vector<xSymbol> attrNames = pAttr.attributeNames();
  if (attrNames.size() != 0)
    pOS << " <";

  for (int i = 0; i < attrNames.size(); ++i) {
    if (i != 0)
      pOS << ", ";

    xSymbol name = attrNames[i];
    pOS << name.toString() << ":";
    switch (pAttr.kindOf(name)) {
      case xAttributeKind::f:
        pOS << "FLOAT " << static_cast<float>(pAttr.f(name));
        break;
      case xAttributeKind::fs: {
        pOS << "FLOATS [";
        auto fs = pAttr.fs(name);
        for (int i = 0; i < fs.size(); ++i) {
          if (i != 0)
            pOS << ",";
          pOS << static_cast<float>(fs[i]) << ", ";
        }
        pOS << "]";
        break;
      }
      case xAttributeKind::i:
        pOS << "INT " << pAttr.i(name);
        break;
      case xAttributeKind::is: {
        pOS << "INTS [";
        auto is = pAttr.is(name);
        for (int i = 0; i < is.size(); ++i) {
          if (i != 0)
            pOS << ",";
          pOS << is[i];
        }
        pOS << "]";
        break;
      }
      case xAttributeKind::s:
        pOS << "STRING ";
        pOS << pAttr.s(name);
        break;
      case xAttributeKind::ss: {
        pOS << "STRINGS [";
        auto ss = pAttr.ss(name);
        for (int i = 0; i < ss.size(); ++i) {
          if (i != 0)
            pOS << ",";
          pOS << ss[i] << ",";
        }
        pOS << "]";
        break;
      }
      case xAttributeKind::t: {
        pOS << "TENSOR " << pAttr.t(name).name();
        break;
      }
      case xAttributeKind::ts: {
        pOS << "TENSORS [";
        auto ts = pAttr.ts(name);
        for (int i = 0; i < ts.size(); ++i) {
          if (i != 0)
            pOS << ",";
          pOS << ts[i].name() << ",";
        }
        pOS << "]";
        break;
      }
      case xAttributeKind::g: {
        pOS << "GRAPH " << pAttr.g(name).get()->name();
        break;
      }
      case xAttributeKind::gs: {
        pOS << "GRAPHS [";
        auto gs = pAttr.gs(name);
        for (int i = 0; i < gs.size(); ++i) {
          if (i != 0)
            pOS << ",";
          pOS << gs[i].get()->name() << ",";
        }
        pOS << "]";
        break;
      }
    } // end of switch
    if (attrNames.size() != 0)
      pOS << "> ";
  } // end of for
}

//===----------------------------------------------------------------------===//
// Module
//===----------------------------------------------------------------------===//
Module::Module()
  : m_RootTensorGraph(),
    m_TensorGraphs(),
    m_OnnxInfo(),
    m_OnnxSetId(),
    m_OnnxMetaData(),
    m_pRootComputeGraph(nullptr),
    m_ComputeGraphs() {
}

Module::Module(std::unique_ptr<xGraph> pGraph)
  : m_RootTensorGraph(std::move(pGraph)),
    m_TensorGraphs(),
    m_OnnxInfo(),
    m_OnnxSetId(),
    m_OnnxMetaData(),
    m_pRootComputeGraph(nullptr),
    m_ComputeGraphs() {
}

Module::~Module()
{
  // Supposedly subgraph can be deleted by destructing top-level graph
  if (1 < m_RootTensorGraph.use_count()) {
    // display error because Module should response for the life cycle of IR.
    error(onnx_graph_alive);
  }
  m_RootTensorGraph.reset();

  for (auto entry : m_Values) {
    Value* v = entry.value();
    delete v;
  }
  m_Values.clear();
}

Module& Module::delegate(std::unique_ptr<xGraph> pGraph)
{
  if (m_RootTensorGraph)
    m_RootTensorGraph.reset();
  return delegate(*pGraph.release());
}

Module& Module::delegate(xGraph& pGraph)
{
  m_RootTensorGraph.reset(&pGraph);

  bool exist = false;
  TensorGraphList::entry_type* entry = nullptr;
  if (m_RootTensorGraph->has_name())
    entry = m_TensorGraphs.insert(m_RootTensorGraph->name(), exist);
  else
    entry = m_TensorGraphs.insert("top-level", exist);
  entry->setValue(m_RootTensorGraph.get());
  return *this;
}

bool Module::recordSubgraph(xGraph& pSubgraph)
{
  bool exist = false;
  TensorGraphList::entry_type* entry = m_TensorGraphs.insert(pSubgraph.name(), exist);
  if (exist)
    return false;

  entry->setValue(&pSubgraph);
  return true;
}

ComputeGraph* Module::getComputeGraph(StringRef pName)
{
  if (m_ComputeGraphs.empty())
    return nullptr;
  ComputeGraphList::iterator cg = m_ComputeGraphs.find(pName);
  if (m_ComputeGraphs.end() == cg)
    return nullptr;
  return cg->value();
}

const ComputeGraph* Module::getComputeGraph(StringRef pName) const
{
  if (m_ComputeGraphs.empty())
    return nullptr;
  ComputeGraphList::const_iterator cg = m_ComputeGraphs.find(pName);
  if (m_ComputeGraphs.end() == cg)
    return nullptr;
  return cg->value();
}

ComputeGraph* Module::createComputeGraph(StringRef pName)
{
  // Note: there are two instances of the graph name.
  // One is in m_ComputeGraphs, another is in ComputeGraph object
  bool exist = false;
  ComputeGraphList::entry_type* entry = m_ComputeGraphs.insert(pName, exist);
  if (exist)
    return nullptr;

  entry->setValue(new ComputeGraph(pName, *this, m_ComputeOperands));
  if (!hasRootComputeGraph())
    m_pRootComputeGraph = entry->value();
  return entry->value();
}

void Module::addValue(Value* pValue)
{
  bool exist = false;
  auto* entry = m_Values.insert(pValue->getName(), exist);
  if (exist)
    return;
  entry->setValue(pValue);
}

Module::ValueList& Module::getValueList()
{
  return m_Values;
}

const Module::ValueList& Module::getValueList() const
{
  return m_Values;
}

void Module::print(std::ostream& pOS) const
{
  if (!hasGraphIR()) {
    pOS << "empty" << std::endl;
    return;
  }

  pOS << "graph " << getRootTensorGraph()->name() << "{\n";

  // XXX: This is ONNX's failure. They forget to write a constant
  // version of ::onnx::Graph::initializer_names()
  xGraph* graph = const_cast<xGraph*>(getRootTensorGraph());

  // dump graph initializers
  pOS << "  initializers: {\n";

  int i = 0;
  while (i < graph->initializers().size()) {
    pOS << "    tensor <";
    for (int64_t size : graph->initializers()[i].sizes()) {
      pOS << size << " ";
    }
    pOS << "> %" << graph->initializer_names()[i];
    ++i;
    if (i < graph->initializers().size())
      pOS << ",\n";
    else
      pOS << "\n";
  }
  pOS << "  },\n";

  // dump graph inputs
  pOS << "  inputs : {\n";
  i = 0;
  while (i < getGraphIR()->inputs().size()) {
    const xValue* v = getGraphIR()->inputs()[i];
    pOS << "    ";
    print(pOS, *v);
    ++i;
    if (i < getGraphIR()->inputs().size())
      pOS << ",\n";
    else
      pOS << "\n";
  }
  pOS << "  }\n";
  pOS << "}\n";

  // dump graph nodes
  for (const xNode *n : getGraphIR()->nodes()) {
    if (xBuiltinSymbol::kUndefined == n->kind())
      continue;

    print(pOS, *n);
  }

  // dump graph outputs
  pOS << "  return ";
  for (int i = 0; i < getGraphIR()->outputs().size(); i++) {
    if (i != 0)
      pOS << ", ";

    const xValue *v = getGraphIR()->outputs()[i];
    print(pOS, *v);
  }
  pOS << std::endl;
}

template<>
void Module::print(std::ostream& pOS, const xValue& pValue) const
{
  pOS << TensorProto_DataType_Name(pValue.elemType()) << " tensor ";
  // print dimension
  auto sizes = pValue.sizes();
  pOS << "<";
  for (size_t i = 0; i < sizes.size(); ++i) {
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
  pOS << '%' << pValue.uniqueName();
}

template<>
void Module::print(std::ostream& pOS, const xNode& pNode) const
{
  // print name
  // XXX: This is ONNX's bug. They forget to write constant getters.
  xNode* node = const_cast<xNode*>(&pNode);
  if (node->has_name())
    pOS << "[" << node->name() << "] ";

  // print outputs.
  for (int i = 0; i < pNode.outputs().size(); ++i) {
    if (i != 0)
      pOS << ", ";

    const xValue *v = pNode.outputs()[i];
    print(pOS, *v);
  }

  pOS << " = " << pNode.kind().toString();

  // print attributes.
  PrintAttrs(pOS, pNode);

  // print inputs.
  pOS << '(';
  for (int i = 0; i < pNode.inputs().size(); ++i) {
    if (i != 0)
      pOS << ", ";

    const xValue *v = pNode.inputs()[i];
    print(pOS, *v);
  }
  pOS << ")" << std::endl;
}

template<>
void Module::print<Module::OpsetImport>(std::ostream& pOS) const
{
  for (auto set_id : m_OnnxSetId) {
    pOS << "opset_import.domain:" << set_id.first << "\n";
    pOS << "opset_import.version:" << set_id.second << "\n";
  }
}

template<>
void Module::print<Module::MetaDataMap>(std::ostream& pOS) const
{
  for (auto meta_data : m_OnnxMetaData) {
    errs() << "metadata_props.key:" << meta_data.first << "\n";
    errs() << "metadata_props.value:" << meta_data.second << "\n";
  }
}

void Module::dump() const
{
  print(errs());
}

//===----------------------------------------------------------------------===//
// Module::OnnxInfo
//===----------------------------------------------------------------------===//
Module::OnnxInfo::OnnxInfo()
  : m_IRVersion(0),
    m_ProducerName(),
    m_ProducerVersion(),
    m_Domain(),
    m_ModelVersion(0),
    m_DocString() {
}

void Module::OnnxInfo::setProducer(const std::string& pName,
                                   const std::string& pVersion)
{
  m_ProducerName = pName;
  m_ProducerVersion = pVersion;
}

void Module::OnnxInfo::print(std::ostream& pOS) const
{
  pOS << "ir_version:" << m_IRVersion << "\n";
  pOS << "producer_name:" << m_ProducerName << "\n";
  pOS << "producer_version:" << m_ProducerVersion << "\n";
  pOS << "domain:" << m_Domain << "\n";
  pOS << "model_version:" << m_ModelVersion << "\n";
  pOS << "doc_string:" << m_DocString << "\n";
}

void Module::OnnxInfo::dump() const
{
  print(errs());
}
