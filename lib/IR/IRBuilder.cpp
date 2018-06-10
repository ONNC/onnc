//===- IRBuilder.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// IRBuilder
//===----------------------------------------------------------------------===//
IRBuilder::IRBuilder(Module& pModule)
  : m_Module(pModule) {
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
}
