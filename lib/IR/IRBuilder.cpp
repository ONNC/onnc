//===- IRBuilder.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/IRBuilder.h>
#include <onnc/Config/ONNX.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// IRBuilder
//===----------------------------------------------------------------------===//
IRBuilder::IRBuilder(Module& pModule)
  : m_InsertionPoint() {
  m_InsertionPoint.setModule(pModule);
}

IRBuilder::IRBuilder(Module& pModule, const xProto& pProto)
  : m_InsertionPoint() {
  m_InsertionPoint.setModule(pModule);
  update(pProto);
}

void IRBuilder::setInsertionPoint(const InsertionPoint& pISP)
{
  m_InsertionPoint = pISP;
}

void IRBuilder::update(const xProto& pProto)
{
  getModule().delegate(xImportModelProto(pProto));

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

  getInsertionPoint().setTensorGraph(*getModule().getRootTensorGraph());
}

void IRBuilder::setComputeGraph(ComputeGraph* pCG)
{
  getInsertionPoint().setComputeGraph(pCG);
}

/// create a tensor graph
xGraph* IRBuilder::CreateTensorGraph()
{
  xGraph* result = new xGraph();
  getInsertionPoint().setTensorGraph(*result);
  getModule().delegate(*getTensorGraph());
  return getTensorGraph();
}

/// create a tensor graph whose name is @ref pName
xGraph* IRBuilder::CreateTensorGraph(StringRef pName)
{
  xGraph* result = new xGraph();
  getInsertionPoint().setTensorGraph(*result);
  getModule().delegate(*getTensorGraph());
  getTensorGraph()->setName(pName);
  return getTensorGraph();
}

xValue* IRBuilder::AddInput(const std::string& pName,
                                   const std::vector<xDimension>& pSizes,
                                   onnc::Value::Type pKind)
{
  if (!hasTensorGraph())
    return nullptr;

  bool exist = false;
  InsertionPoint::CreatedValues::entry_type* entry =
      getInsertionPoint().getCreatedValues().insert(pName, exist);

  /// the created input should be unique
  if (exist)
    return nullptr;

  xValue *result = getTensorGraph()->addInput();
  result->setUniqueName(pName)
        ->setSizes(pSizes)
        ->setElemType((xTensorProtoDataType)pKind);

  entry->setValue(result);
  return result;
}

xNode*
IRBuilder::AddNode(const std::string& pName, const StringList& pInputNames)
{
  if (!hasTensorGraph())
    return nullptr;

  xNode *node = getTensorGraph()->create(xSymbol(pName));
  // make sure every input name exists
  for (auto & input : pInputNames) {
    if (getInsertionPoint().getCreatedValues().end() ==
        getInsertionPoint().getCreatedValues().find(input))
      return nullptr;
  }

  // complete the node's inputs
  for (auto & input : pInputNames) {
    node->addInput(getInsertionPoint().getCreatedValues().find(input)->value());
  }

  getTensorGraph()->appendNode(node);
  getInsertionPoint().setTensorNode(*node);
  return node;
}

xNode* IRBuilder::CloneNode(xNode& pNode, const std::string& pName)
{
  xGraph* graph = pNode.owningGraph();

  // kind and graph
  xNode *node = graph->create(pNode.kind());

  // name
  if (!pName.empty())
    node->setName(pName);

  // copy doc_strings
  std::string doc_string(pNode.docString());
  node->setDocString(doc_string);

  // attributes
  node->copyAttributes(pNode);

  getInsertionPoint().setTensorNode(*node);
  return node;
}

onnc::InitializerProxy
IRBuilder::AddInitializer(const std::string& pName,
                          const std::vector<xDimension>& pSizes,
                          onnc::Value::Type pKind)
{
  if (!hasTensorGraph())
    return InitializerProxy();

  // XXX: using local variables because xGraph::addInitializer does
  // delegation (through std::move).
  xTensor t;
  t.elem_type() = (xTensorProtoDataType)pKind;
  t.setName(pName);

  if (pSizes.empty()) { // find from created values
    bool exist = false;
    InsertionPoint::CreatedValues::entry_type* entry =
        getInsertionPoint().getCreatedValues().insert(pName, exist);
    if (exist) {
      t.sizes().reserve(entry->value()->sizes().size());
      for (xDimension d : entry->value()->sizes())
        t.sizes().push_back(d.dim);
    }
  }
  else {
    t.sizes().reserve(pSizes.size());
    for (xDimension d : pSizes)
      t.sizes().push_back(d.dim);
  }

  getTensorGraph()->addInitializer(t, pName);

  // XXX: using back() because xGraph::addInitializer is an appending.
  onnc::InitializerProxy result(getTensorGraph()->initializer_names().back(),
                                getTensorGraph()->initializers().back());
  return result;
}

xValue*
IRBuilder::AddOutput(const std::string& pName,
                     const std::vector<xDimension>& pSizes,
                     onnc::Value::Type pKind)
{
  if (!hasTensorNode())
    return nullptr;

  bool exist = false;
  InsertionPoint::CreatedValues::entry_type* entry =
      getInsertionPoint().getCreatedValues().insert(pName, exist);

  /// the created output value should be unique
  if (exist)
    return nullptr;

  /// XXX: ONNX has already created a default output value. If we are
  /// in the very first adding, then use the created one.
  xValue *out = nullptr;
  if (1 < getTensorNode()->outputs().size())
    out = getTensorNode()->addOutput();
  else
    out = getTensorNode()->output();

  out->setUniqueName(pName)
     ->setSizes(pSizes)
     ->setElemType((xTensorProtoDataType)pKind);

  // books the value
  entry->setValue(out);

  return out;
}

bool IRBuilder::FinalizeTensorGraph(const StringList& pOutputs)
{
  for (auto & output : pOutputs) {
    if (getInsertionPoint().getCreatedValues().end() ==
        getInsertionPoint().getCreatedValues().find(output))
      return false;
  }

  for (auto & output : pOutputs) {
    // register graph output
    getTensorGraph()->registerOutput(
        getInsertionPoint().getCreatedValues().find(output)->value());
  }
  return true;
}

#define CREATE_VAL_DATA(result, CG, tensor, ONNCType, NativeType, accessor) \
{ \
  auto t = CG.addValue<ONNCType>(name); \
  /* copy tensor init data. */ \
  if (tensor.is_raw_data()) { \
    const size_t numElems = tensor.raw().size() / sizeof(NativeType); \
    NativeType* d = (NativeType*)tensor.raw().c_str(); \
    t->getValues().resize(numElems); \
    for (size_t i = 0; i < numElems; ++i) \
      t->getValues()[i] = d[i]; \
  } \
  else { \
    const size_t numElems = tensor.accessor().size(); \
    t->getValues().resize(numElems); \
    for (int i = 0; i < numElems; ++i) \
      t->getValues()[i] = tensor.accessor()[i]; \
  } \
  result = t; \
}

Tensor* IRBuilder::CreateComputeTensor(ComputeGraph& pCG,
                                       const xValue& pValue,
                                       const xTensor& pTensor)
{
  const std::string &name = pValue.uniqueName();
  Tensor* result = nullptr;
  switch (pValue.elemType()) {
  case onnc::Value::kInt8: {
    CREATE_VAL_DATA(result, pCG, pTensor, Int8Tensor, int32_t, int32s);
    break;
  }
  case onnc::Value::kInt16: {
    CREATE_VAL_DATA(result, pCG, pTensor, Int16Tensor, int32_t, int32s);
    break;
  }
  case onnc::Value::kInt32: {
    CREATE_VAL_DATA(result, pCG, pTensor, Int32Tensor, int32_t, int32s);
    break;
  }
  case onnc::Value::kInt64: {
    CREATE_VAL_DATA(result, pCG, pTensor, Int64Tensor, int64_t, int64s);
    break;
  }
  case onnc::Value::kUint8: {
    CREATE_VAL_DATA(result, pCG, pTensor, Uint8Tensor, int32_t, int32s);
    break;
  }
  case onnc::Value::kUint16: {
    CREATE_VAL_DATA(result, pCG, pTensor, Uint16Tensor, int32_t, int32s);
    break;
  }
  case onnc::Value::kUint32: {
    CREATE_VAL_DATA(result, pCG, pTensor, Uint32Tensor, uint64_t, uint64s);
    break;
  }
  case onnc::Value::kUint64: {
    CREATE_VAL_DATA(result, pCG, pTensor, Uint64Tensor, uint64_t, uint64s);
    break;
  }
  case onnc::Value::kFloat: {
    CREATE_VAL_DATA(result, pCG, pTensor, FloatTensor, float, floats);
    break;
  }
  case onnc::Value::kFloat16: {
    CREATE_VAL_DATA(result, pCG, pTensor, Float16Tensor, float, floats);
    break;
  }
  case onnc::Value::kString: {
    CREATE_VAL_DATA(result, pCG, pTensor, StringTensor, std::string, strings);
    break;
  }
  case onnc::Value::kBoolean: {
    CREATE_VAL_DATA(result, pCG, pTensor, BooleanTensor, int32_t, int32s);
    break;
  }
  case onnc::Value::kDouble: {
    CREATE_VAL_DATA(result, pCG, pTensor, DoubleTensor, double, doubles);
    break;
  }
  default:
    errs() << "createTensor error: unknow elemtype = "
           << pValue.elemType() << "\n";
    return nullptr;
  }

  std::vector<int64_t> sizes(pValue.sizes().size());
  for (int i = 0; i < pValue.sizes().size(); ++i)
    sizes[i] = pValue.sizes()[i].dim;
  result->setDimensions(sizes);
  return result;
}

Tensor* IRBuilder::CreateComputeTensor(const xValue& pValue,
                                       const xTensor& pTensor)
{
  return CreateComputeTensor(*getComputeGraph(), pValue, pTensor);
}

Tensor* IRBuilder::CreateComputeTensor(ComputeGraph& pCG, const xValue& pValue)
{
  Tensor* result = nullptr;
  switch (pValue.elemType()) {
  case onnc::Value::kInt8: {
    result = pCG.addValue<onnc::Int8Tensor>(pValue.uniqueName());
    break;
  }
  case onnc::Value::kInt16: {
    result = pCG.addValue<onnc::Int16Tensor>(pValue.uniqueName());
    break;
  }
  case onnc::Value::kInt32: {
    result = pCG.addValue<onnc::Int32Tensor>(pValue.uniqueName());
    break;
  }
  case onnc::Value::kInt64: {
    result = pCG.addValue<onnc::Int64Tensor>(pValue.uniqueName());
    break;
  }
  case onnc::Value::kUint8: {
    result = pCG.addValue<onnc::Uint8Tensor>(pValue.uniqueName());
    break;
  }
  case onnc::Value::kUint16: {
    result = pCG.addValue<onnc::Uint16Tensor>(pValue.uniqueName());
    break;
  }
  case onnc::Value::kUint32: {
    result = pCG.addValue<onnc::Uint32Tensor>(pValue.uniqueName());
    break;
  }
  case onnc::Value::kUint64: {
    result = pCG.addValue<onnc::Uint64Tensor>(pValue.uniqueName());
    break;
  }
  case onnc::Value::kFloat: {
    result = pCG.addValue<onnc::FloatTensor>(pValue.uniqueName());
    break;
  }
  case onnc::Value::kFloat16: {
    result = pCG.addValue<onnc::Float16Tensor>(pValue.uniqueName());
    break;
  }
  case onnc::Value::kString: {
    result = pCG.addValue<onnc::StringTensor>(pValue.uniqueName());
    break;
  }
  case onnc::Value::kBoolean: {
    result = pCG.addValue<onnc::BooleanTensor>(pValue.uniqueName());
    break;
  }
  case onnc::Value::kDouble: {
    result = pCG.addValue<onnc::DoubleTensor>(pValue.uniqueName());
    break;
  }
  default:
    errs() << "createTensor error: unknow elemtype = "
           << pValue.elemType() << "\n";
    return nullptr;
  }

  std::vector<int64_t> sizes(pValue.sizes().size());
  for (int i = 0; i < pValue.sizes().size(); ++i)
    sizes[i] = pValue.sizes()[i].dim;
  result->setDimensions(sizes);
  return result;
}

Tensor* IRBuilder::CreateComputeTensor(const xValue& pValue)
{
  return IRBuilder::CreateComputeTensor(*getComputeGraph(), pValue);
}
