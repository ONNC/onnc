#include <onnc/IR/Module.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnx/common/ir_pb_converter.h>

namespace onnc {
namespace onnx {

void SerializeToString(std::string &output, const Module &pModule)
{
  ::onnx::ModelProto modelProto;
  ExportModelProto(modelProto, pModule);
  modelProto.SerializeToString(&output);
}

void ExportModelProto(::onnx::ModelProto &pModelProto, const Module &pModule)
{
  pModelProto.set_ir_version(pModule.getOnnxInfo().getIRVersion());
  pModelProto.set_producer_name(pModule.getOnnxInfo().getProducerName());
  pModelProto.set_producer_version(pModule.getOnnxInfo().getProducerVersion());
  pModelProto.set_domain(pModule.getOnnxInfo().getDomain());
  pModelProto.set_model_version(pModule.getOnnxInfo().getModelVersion());
  pModelProto.set_doc_string(pModule.getOnnxInfo().getDocString());
  ::onnx::ExportModelProto(&pModelProto,
                          const_cast<Module &>(pModule).getGraph());
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

void ImportModelProto(Module &pModule, const ::onnx::ModelProto &pModelProto)
{
  if (pModelProto.has_ir_version())
    pModule.getOnnxInfo().setIRVersion(pModelProto.ir_version());

  if (pModelProto.has_producer_name())
    pModule.getOnnxInfo().setProducerName(pModelProto.producer_name());

  if (pModelProto.has_producer_version())
    pModule.getOnnxInfo().setProducerVersion(pModelProto.producer_version());

  if (pModelProto.has_domain())
    pModule.getOnnxInfo().setDomain(pModelProto.domain());

  if (pModelProto.has_model_version())
    pModule.getOnnxInfo().setModelVersion(pModelProto.model_version());

  if (pModelProto.has_doc_string())
    pModule.getOnnxInfo().setDocString(pModelProto.doc_string());

  pModule.delegateGraph(::onnx::ImportModelProto(pModelProto));

  for (int i = 0; i < pModelProto.opset_import_size(); i++) {
    auto &opset = pModelProto.opset_import(i);
    pModule.getSetId().insert({ opset.domain(), opset.version() });
  }

  for (int i = 0; i < pModelProto.metadata_props_size(); i++) {
    auto &strStrEntry = pModelProto.metadata_props(i);
    pModule.getMetaData().insert({ strStrEntry.key(), strStrEntry.value() });
  }
}

size_t getTotalCount(const std::vector<int64_t> &pDim)
{
  size_t s = 1;
  for (auto &size : pDim)
    s *= size;
  return s;
}

const ::onnx::Tensor &getTensor(std::string name, const ::onnx::Graph &graph)
{
  auto initNames = const_cast< ::onnx::Graph &>(graph).initializer_names();
  std::ptrdiff_t idx = std::distance(
      initNames.begin(), std::find(initNames.begin(), initNames.end(), name));
  return const_cast< ::onnx::Graph &>(graph).initializers()[idx];
}

} // namespace onnx
} // namespace onnc
