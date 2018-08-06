//===- ONNXUtils.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Module.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Config/ONNX.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Procedures
//===----------------------------------------------------------------------===//
void onnc::SerializeToString(std::string &output, const Module &pModule)
{
  xProto modelProto;
  ExportModelProto(modelProto, pModule);
  modelProto.SerializeToString(&output);
}

void onnc::ExportModelProto(xProto &pModelProto, const Module &pModule)
{
  pModelProto.set_ir_version(pModule.getOnnxInfo().getIRVersion());
  pModelProto.set_producer_name(pModule.getOnnxInfo().getProducerName());
  pModelProto.set_producer_version(pModule.getOnnxInfo().getProducerVersion());
  pModelProto.set_domain(pModule.getOnnxInfo().getDomain());
  pModelProto.set_model_version(pModule.getOnnxInfo().getModelVersion());
  pModelProto.set_doc_string(pModule.getOnnxInfo().getDocString());
  xExportModelProto(&pModelProto,
      std::shared_ptr<xGraph>(const_cast<Module &>(pModule).getGraphIR()));
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

Module* onnc::CreateModule(const xProto &pModelProto)
{
  Module* module = new Module(xImportModelProto(pModelProto));

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

const xTensor &onnc::getTensor(std::string name, const xGraph &pGraph)
{
  auto &graph = const_cast<xGraph &>(pGraph);
  auto initNames = graph.initializer_names();
  std::ptrdiff_t idx = std::distance(
      initNames.begin(), std::find(initNames.begin(), initNames.end(), name));
  if (idx >= graph.initializers().size()) {
    std::cerr << "error: can't find weight " << name << "!" << std::endl;
    exit(1);
  }
  return graph.initializers()[idx];
}

bool onnc::OutputSizeIsInputSize(xNode& pNode)
{
  /// Operator set whose output size equals to input size.
  /// (FIXME: Currently, this also implies: y[i] = some operation on x[i])
  const static std::unordered_set<xNodeKind> g_OutputSizeIsInputSize = {
    xSymbol("Relu"), xSymbol("LRN"),
    xSymbol("Dropout"), xSymbol("Softmax"),
    xBuiltinSymbol::kBatchNormalization,
    xBuiltinSymbol::kMul,
    xBuiltinSymbol::kDiv,
    xBuiltinSymbol::kAdd,
    xBuiltinSymbol::kSub,
    xBuiltinSymbol::kNeg,
    xSymbol("Sum")
  };

  return g_OutputSizeIsInputSize.count(pNode.kind()) != 0;
}

void onnc::GetAttrVals(xNode& pNode, xBuiltinSymbol pAttr,
                        LongInts& pVal)
{
  if (pNode.hasAttribute(pAttr)) {
    const auto &attr = pNode.is(pAttr);
    pVal.resize(attr.size());
    for (int i = 0; i < attr.size(); ++i)
      pVal[i] = attr[i];
  }
}

void onnc::GetPads(xNode& pNode, LongInts& pPadsB, LongInts& pPadsE)
{
  if (pNode.hasAttribute(xBuiltinSymbol::kpads)) {
    // get pads begin and offset to pads end.
    const auto& pads = pNode.is(xBuiltinSymbol::kpads);
    const size_t padEndOffset = pads.size() / 2;

    pPadsB.resize(padEndOffset);
    pPadsE.resize(padEndOffset);
    for (int i = 0; i < padEndOffset; ++i) {
      pPadsB[i] = pads[i];
      pPadsE[i] = pads[i + padEndOffset];
    }
  }
}

void onnc::GetConvKernelShape(xNode& pNode, LongInts& pKShape)
{
  assert(pNode.kind() ==
         xBuiltinSymbol::kConv && "This is not a convolution node.");
  const TensorSizes& wDim = pNode.inputs()[1]->sizes();
  const size_t numAxis = wDim.size() - 2;

  if (pNode.hasAttribute(xBuiltinSymbol::kkernel_shape)) {
    GetAttrVals(pNode, xBuiltinSymbol::kkernel_shape, pKShape);
  } else {
    pKShape.resize(numAxis);
    // If the kernel shape is not present, it should be inferred from input W.
    for (int i = 0; i < numAxis; ++i)
      pKShape[i] = wDim[i + 2].dim;
  }
}

/// @param pAttr Can be '::onnx::ktransA', '::onnx::ktransB'
bool onnc::IsTranspose(const xNode& pNode, const xBuiltinSymbol pAttr)
{
  assert((pAttr == xBuiltinSymbol::ktransA || pAttr == xBuiltinSymbol::ktransB) &&
         "This is not transpose attribute.");
  if (pNode.hasAttribute(pAttr) &&
      pNode.i(pAttr))
    return true;
  return false;
}

LongInts onnc::GetValueSizes(const xValue& pVal)
{
  LongInts sizes;

  // General case: N C H W.
  sizes.reserve(4);
  for (auto & dim: pVal.sizes())
    sizes.emplace_back(dim.dim);
  return sizes;
}
