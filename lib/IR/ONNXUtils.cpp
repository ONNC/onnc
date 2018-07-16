#include <onnc/IR/Module.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnx/common/ir_pb_converter.h>

using namespace onnc;

void onnc::SerializeToString(std::string &output, const Module &pModule)
{
  ::onnx::ModelProto modelProto;
  ExportModelProto(modelProto, pModule);
  modelProto.SerializeToString(&output);
}

void onnc::ExportModelProto(::onnx::ModelProto &pModelProto, const Module &pModule)
{
  pModelProto.set_ir_version(pModule.getOnnxInfo().getIRVersion());
  pModelProto.set_producer_name(pModule.getOnnxInfo().getProducerName());
  pModelProto.set_producer_version(pModule.getOnnxInfo().getProducerVersion());
  pModelProto.set_domain(pModule.getOnnxInfo().getDomain());
  pModelProto.set_model_version(pModule.getOnnxInfo().getModelVersion());
  pModelProto.set_doc_string(pModule.getOnnxInfo().getDocString());
  ::onnx::ExportModelProto(&pModelProto,
      std::shared_ptr<::onnx::Graph>(const_cast<Module &>(pModule).getGraphIR()));
  for (const auto &setId : pModule.getSetId()) {
    auto *opset_imports = pModelProto.add_opset_import();
    opset_imports->set_domain(setId.first);
    opset_imports->set_version(setId.second);
  }
  for (const auto &metaData : pModule.getMetaData()) {
    auto *metadata_props = pModelProto.add_metadata_props();
    metadata_props->set_key(metaData.first);
    metadata_props->set_value(metaData.second);
  }
}

Module* onnc::CreateModule(const ::onnx::ModelProto &pModelProto)
{
  Module* module = new Module(::onnx::ImportModelProto(pModelProto));

  if (pModelProto.has_ir_version())
    module->getOnnxInfo().setIRVersion(pModelProto.ir_version());

  if (pModelProto.has_producer_name())
    module->getOnnxInfo().setProducerName(pModelProto.producer_name());

  if (pModelProto.has_producer_version())
    module->getOnnxInfo().setProducerVersion(pModelProto.producer_version());

  if (pModelProto.has_domain())
    module->getOnnxInfo().setDomain(pModelProto.domain());

  if (pModelProto.has_model_version())
    module->getOnnxInfo().setModelVersion(pModelProto.model_version());

  if (pModelProto.has_doc_string())
    module->getOnnxInfo().setDocString(pModelProto.doc_string());

  for (int i = 0; i < pModelProto.opset_import_size(); i++) {
    auto &opset = pModelProto.opset_import(i);
    module->getSetId().insert({ opset.domain(), opset.version() });
  }

  for (int i = 0; i < pModelProto.metadata_props_size(); i++) {
    auto &strStrEntry = pModelProto.metadata_props(i);
    module->getMetaData().insert({ strStrEntry.key(), strStrEntry.value() });
  }

  return module;
}

void onnc::DestroyModule(Module*& pModule)
{
  if (nullptr == pModule)
    return;
  delete pModule;
  pModule = nullptr;
}

size_t onnc::getTotalCount(const std::vector<int64_t> &pDim)
{
  size_t s = 1;
  for (auto &size : pDim)
    s *= size;
  return s;
}

const ::onnx::Tensor &onnc::getTensor(std::string name,
                                      const ::onnx::Graph &pGraph)
{
  auto &graph = const_cast< ::onnx::Graph &>(pGraph);
  auto initNames = graph.initializer_names();
  std::ptrdiff_t idx = std::distance(
      initNames.begin(), std::find(initNames.begin(), initNames.end(), name));
  if (idx >= graph.initializers().size()) {
    std::cerr << "error: can't find weight " << name << "!" << std::endl;
    exit(1);
  }
  return graph.initializers()[idx];
}

bool onnc::OutputSizeIsInputSize(::onnx::Node& pNode)
{
  /// Operator set whose output size equals to input size.
  /// (FIXME: Currently, this also implies: y[i] = some operation on x[i])
  const static std::unordered_set<::onnx::NodeKind> g_OutputSizeIsInputSize = {
    ::onnx::Symbol("Relu"), ::onnx::Symbol("LRN"),
    ::onnx::Symbol("Dropout"), ::onnx::Symbol("Softmax"),
    ::onnx::kBatchNormalization,
    ::onnx::kMul, ::onnx::kDiv, ::onnx::kAdd, ::onnx::kSub, ::onnx::kNeg,
    ::onnx::Symbol("Sum")
  };

  return g_OutputSizeIsInputSize.count(pNode.kind()) != 0;
}

void onnc::GetAttrVals(::onnx::Node& pNode, ::onnx::BuiltinSymbol pAttr,
                        LongInts& pVal)
{
  if (pNode.hasAttribute(pAttr)) {
    const auto &attr = pNode.is(pAttr);
    pVal.resize(attr.size());
    for (int i = 0; i < attr.size(); ++i)
      pVal[i] = attr[i];
  }
}

void onnc::GetPads(::onnx::Node& pNode, LongInts& pPadsB, LongInts& pPadsE)
{
  if (pNode.hasAttribute(::onnx::kpads)) {
    // get pads begin and offset to pads end.
    const auto& pads = pNode.is(::onnx::kpads);
    const size_t padEndOffset = pads.size() / 2;

    pPadsB.resize(padEndOffset);
    pPadsE.resize(padEndOffset);
    for (int i = 0; i < padEndOffset; ++i) {
      pPadsB[i] = pads[i];
      pPadsE[i] = pads[i + padEndOffset];
    }
  }
}

void onnc::GetConvKernelShape(::onnx::Node& pNode, LongInts& pKShape)
{
  assert(pNode.kind() == ::onnx::kConv && "This is not a convolution node.");
  const TensorSizes& wDim = pNode.inputs()[1]->sizes();
  const size_t numAxis = wDim.size() - 2;

  if (pNode.hasAttribute(::onnx::kkernel_shape)) {
    GetAttrVals(pNode, ::onnx::kkernel_shape, pKShape);
  } else {
    pKShape.resize(numAxis);
    // If the kernel shape is not present, it should be inferred from input W.
    for (int i = 0; i < numAxis; ++i)
      pKShape[i] = wDim[i + 2].dim;
  }
}

/// @param pAttr Can be '::onnx::ktransA', '::onnx::ktransB'
bool onnc::IsTranspose(const ::onnx::Node& pNode, const ::onnx::BuiltinSymbol pAttr)
{
  assert((pAttr == ::onnx::ktransA || pAttr == ::onnx::ktransB) &&
         "This is not transpose attribute.");
  if (pNode.hasAttribute(pAttr) &&
      pNode.i(pAttr))
    return true;
  return false;
}

LongInts onnc::GetValueSizes(const ::onnx::Value& pVal)
{
  LongInts sizes;

  // General case: N C H W.
  sizes.reserve(4);
  for (auto & dim: pVal.sizes())
    sizes.emplace_back(dim.dim);
  return sizes;
}
