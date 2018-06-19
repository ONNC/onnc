//===- IRBuilder.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/IRBuilder.h>
#include <onnx/onnx.pb.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
static std::vector<::onnx::Dimension> IntsToDim(const std::vector<int>& pSizes)
{
  std::vector<::onnx::Dimension> dims;
  for (int d : pSizes)
    dims.push_back(::onnx::Dimension(d));
  return dims;
}

//===----------------------------------------------------------------------===//
// IRBuilder
//===----------------------------------------------------------------------===//
IRBuilder::IRBuilder(Module& pModule)
  : m_Module(pModule), m_pTargetTG(nullptr), m_pTargetCG(nullptr) {
}

IRBuilder::IRBuilder(Module& pModule, const ::onnx::ModelProto& pProto)
  : IRBuilder(pModule) {
  update(pProto);
}

void IRBuilder::update(const ::onnx::ModelProto& pProto)
{
  getModule().delegate(::onnx::ImportModelProto(pProto));

  if (pProto.has_ir_version())
    getModule().getOnnxInfo().setIRVersion(pProto.ir_version());

  if (pProto.has_producer_name())
    getModule().getOnnxInfo().setProducerName(pProto.producer_name());

  if (pProto.has_producer_version())
    getModule().getOnnxInfo().setProducerVersion(pProto.producer_version());

  if (pProto.has_domain())
    getModule().getOnnxInfo().setDomain(pProto.domain());

  if (pProto.has_model_version())
    getModule().getOnnxInfo().setModelVersion(pProto.model_version());

  if (pProto.has_doc_string())
    getModule().getOnnxInfo().setDocString(pProto.doc_string());

  for (int i = 0; i < pProto.opset_import_size(); i++) {
    auto &opset = pProto.opset_import(i);
    getModule().getSetId().insert({ opset.domain(), opset.version() });
  }

  for (int i = 0; i < pProto.metadata_props_size(); i++) {
    auto &strStrEntry = pProto.metadata_props(i);
    getModule().getMetaData().insert({ strStrEntry.key(), strStrEntry.value() });
  }

  m_pTargetTG = getModule().getGraphIR().get();
}

::onnx::Value* IRBuilder::AddInput(const std::string& pName,
                                   const std::vector<::onnx::Dimension>& pSizes,
                                   onnc::Value::Type pKind)
{
  if (!hasTensorGraph())
    return nullptr;

  bool exist = false;
  CreateValues::entry_type* entry = m_CreatedValues.insert(pName, exist);

  /// the created input should be unique
  if (exist)
    return nullptr;

  ::onnx::Value *result = getTensorGraph()->addInput();
  result->setUniqueName(pName)
        ->setSizes(pSizes)
        ->setElemType((onnx::TensorProto_DataType)pKind);

  entry->setValue(result);
  return result;
}

::onnx::Value* IRBuilder::AddInput(const std::string& pName,
                                   const std::vector<int>& pSizes,
                                   onnc::Value::Type pKind)
{
  return AddInput(pName, IntsToDim(pSizes), pKind);
}

::onnx::Node*
IRBuilder::AddNode(const std::string& pName, const StringList& pInputNames)
{
  if (!hasTensorGraph())
    return nullptr;

  ::onnx::Node *node = getTensorGraph()->create(::onnx::Symbol(pName));
  // make sure every input name exists
  for (auto & input : pInputNames) {
    if (m_CreatedValues.end() == m_CreatedValues.find(input))
      return nullptr;
  }

  // complete the node's inputs
  for (auto & input : pInputNames) {
    node->addInput(m_CreatedValues.find(input)->value());
  }

  getTensorGraph()->appendNode(node);
  m_pTargetTNode = node;
  return node;
}

onnc::Initializer
IRBuilder::AddInitializer(const std::string& pName, onnc::Value::Type pKind)
{
  if (!hasTensorGraph())
    return Initializer();

  // XXX: using local variables because ::onnx::Graph::addInitializer does
  // delegation (through std::move).
  ::onnx::Tensor t;
  t.elem_type() = (::onnx::TensorProto_DataType)pKind;
  t.setName(pName);
  getTensorGraph()->addInitializer(t, pName);

  // XXX: using back() because ::onnx::Graph::addInitializer is an appending.
  onnc::Initializer result(getTensorGraph()->initializer_names().back(),
                           getTensorGraph()->initializers().back());
  return result;
}

::onnx::Value*
IRBuilder::AddOutput(const std::string& pName,
                     const std::vector<::onnx::Dimension>& pSizes,
                     onnc::Value::Type pKind)
{
  if (!hasTensorNode())
    return nullptr;

  bool exist = false;
  CreateValues::entry_type* entry = m_CreatedValues.insert(pName, exist);

  /// the created output value should be unique
  if (exist)
    return nullptr;

  /// XXX: ONNX has already created a default output value. If we are
  /// in the very first adding, then use the created one.
  ::onnx::Value *out = nullptr;
  if (1 < getTensorNode()->outputs().size())
    out = getTensorNode()->addOutput();
  else
    out = getTensorNode()->output();

  out->setUniqueName(pName)
     ->setSizes(pSizes)
     ->setElemType((onnx::TensorProto_DataType)pKind);

  // books the value
  entry->setValue(out);

  return out;
}

bool IRBuilder::FinalizeTensorGraph(const StringList& pOutputs)
{
  for (auto & output : pOutputs) {
    if (m_CreatedValues.end() == m_CreatedValues.find(output))
      return false;
  }

  for (auto & output : pOutputs) {
    // register graph output
    getTensorGraph()->registerOutput(m_CreatedValues.find(output)->value());
  }
  return true;
}
