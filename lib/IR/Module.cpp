//===- Module.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Module.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnx/common/ir.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Module
//===----------------------------------------------------------------------===//
Module::Module()
  : m_pOnnxGraph(),
    m_OnnxInfo(),
    m_OnnxSetId(),
    m_OnnxMetaData(),
    m_ComputeGraphs() {
}

Module::Module(std::unique_ptr< ::onnx::Graph> pGraph)
  : m_pOnnxGraph(std::move(pGraph)),
    m_OnnxInfo(),
    m_OnnxSetId(),
    m_OnnxMetaData(),
    m_ComputeGraphs() {
}

Module::~Module()
{
  if (1 < m_pOnnxGraph.use_count()) {
    // display error because Module should response for the life cycle of IR.
    error(onnx_graph_alive);
  }
  m_pOnnxGraph.reset();
}

Module& Module::delegate(std::unique_ptr< ::onnx::Graph> pGraph)
{
  m_pOnnxGraph = std::move(pGraph);
  return *this;
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
  bool exist = false;
  ComputeGraphList::entry_type* entry = m_ComputeGraphs.insert(pName, exist);
  if (exist)
    return nullptr;

  entry->setValue(new ComputeGraph(*this));
  return entry->value();
}

void Module::print(std::ostream& pOS) const
{
  // TODO
}

template<>
void Module::print(std::ostream& pOS, const ::onnx::Value& pValue) const
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

template<> void
Module::print(std::ostream& pOS,
              const ::onnx::Attributes<::onnx::Node>& pAttr) const
{
  std::vector<::onnx::Symbol> attrNames = pAttr.attributeNames();
  if (attrNames.size() != 0)
    pOS << " <";

  for (int i = 0; i < attrNames.size(); ++i) {
    if (i != 0)
      pOS << ", ";

    ::onnx::Symbol name = attrNames[i];
    pOS << name.toString() << ":";
    switch (pAttr.kindOf(name)) {
      case ::onnx::AttributeKind::f:
        pOS << "FLOAT " << static_cast<float>(pAttr.f(name));
        break;
      case ::onnx::AttributeKind::fs: {
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
      case ::onnx::AttributeKind::i:
        pOS << "INT " << pAttr.i(name);
        break;
      case ::onnx::AttributeKind::is: {
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
      case ::onnx::AttributeKind::s:
        pOS << "STRING ";
        pOS << pAttr.s(name);
        break;
      case ::onnx::AttributeKind::ss: {
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
      case ::onnx::AttributeKind::t: {
        pOS << "TENSOR " << pAttr.t(name).name();
        break;
      }
      case ::onnx::AttributeKind::ts: {
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
      case ::onnx::AttributeKind::g: {
        pOS << "GRAPH " << pAttr.g(name).get()->name();
        break;
      }
      case ::onnx::AttributeKind::gs: {
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

template<>
void Module::print(std::ostream& pOS, const ::onnx::Node& pNode) const
{
  // TODO
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
