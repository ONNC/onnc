//===- BM1880Backend.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "BM188xBackend.h"
#include "BM188x/BM188xTargetMemInfo.h"
#include "BM188x/BM188xTargetTransformInfo.h"
#include "BM188xCodeEmitter.h"
#include "BM188xFuseOptimizer.h"
#include "BM188xISelLowering.h"
#include "Lowers/AveragePoolLower.h"
#include "Lowers/ConvLower.h"
#include "Lowers/GemmLower.h"
#include "Lowers/GlobalAveragePoolLower.h"
#include "Lowers/LRNLower.h"
#include "Lowers/LeakyReluLower.h"
#include "Lowers/StoreLower.h"
#include "Lowers/SumLower.h"
#include "Lowers/ScaleLower.h"
#include "Lowers/ReluLower.h"
#include "TG.h"
#include <google/protobuf/text_format.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/IR/ONNCModulePrinter.h>
#include <onnc/Transforms/RemoveTrainingNodes.h>
#include <onnc/Transforms/TensorSel/LowerRegistry.h>
#ifdef BMONNC_EXIST
#include <bmnetc/bmnetc.h>
#endif

using namespace onnc;

//===----------------------------------------------------------------------===//
// BM1880
//===----------------------------------------------------------------------===//
BM1880Backend::BM1880Backend(const TargetOptions &pOptions)
    : TGBackend(new BM188xISelLowering(this), new BM188xCodeEmitter(this),
                pOptions)
{
  m_pMemInfo = new BM188xTargetMemInfo(this);
  m_pTTI = new BM188xTargetTransformInfo(this);
}

void BM1880Backend::addTensorSel(PassManager &pPM)
{
#ifdef BMONNC_EXIST
  bmnetc_pass_extention(this, pPM);
#endif

  // target independent pass
  pPM.add(CreateRemoveTrainingNodesPass());
  // TODO refactoring, AddDummyWeightPass can be target indepedent pass
  pPM.add(createAddDummyWeightPass(this));
  pPM.add(CreateUpdateGraphOutputSizePass());

  // BM1880 customized Pass
  pPM.add(createPrepareCtablePass(this));
  pPM.add(createONNXFuseOptPass(this));
  if (getOption().m_PrintModuleBeforeSel)
    pPM.add(createONNCModulePrinterPass());

  if (getTargetLower() == nullptr) {
    pPM.add(createTargetLoweringPass(this));
  } else {
    pPM.add(getTargetLower()(this));
  }

  pPM.add(createQuantizePass(this));
  pPM.add(createUpdateCtablePass(this));

  return;
}

bool BM1880Backend::isNativeTensorType(::onnx::TensorProto_DataType pType)
{
  switch (pType) {
  case ::onnx::TensorProto_DataType_INT8:
  case ::onnx::TensorProto_DataType_INT16:
    return true;
  default:
    return false;
  }
}

void BM1880Backend::setCtableProto(const std::string &pTextString)
{
  ::google::protobuf::TextFormat::ParseFromString(pTextString,
                                                  &m_NetCtableParam);
}

tg::bm1880::LayerCalibrationParameter *
BM1880Backend::getMutableLayerCtable(const std::string &pName)
{
  for (int i = 0; i < m_NetCtableParam.layer_size(); i++) {
    tg::bm1880::LayerCalibrationParameter *layer =
        m_NetCtableParam.mutable_layer(i);
    if (layer->name() == pName) {
      return layer;
    }
  }
  return nullptr;
}

const tg::bm1880::LayerCalibrationParameter *
BM1880Backend::getLayerCtable(const std::string &pName)
{
  for (int i = 0; i < m_NetCtableParam.layer_size(); i++) {
    const tg::bm1880::LayerCalibrationParameter &layer =
        m_NetCtableParam.layer(i);
    for (int j = 0; j < layer.blob_param_size(); j++) {
      if (layer.blob_param(j).name() == pName) {
        return &layer;
      }
    }
  }
  return nullptr;
}

std::unique_ptr<TGFuseOptimizer> BM1880Backend::getFuseOptimizr()
{
  return std::make_unique<BM188xFuseOptimizer>(this);
}

void BM1880Backend::RegisterLowers(LowerRegistry& pRegistry) const
{
  pRegistry.emplace<BM188X::AveragePoolLower>();
  pRegistry.emplace<BM188X::ConvLower>();
  pRegistry.emplace<BM188X::GemmLower>();
  pRegistry.emplace<BM188X::GlobalAveragePoolLower>();
  pRegistry.emplace<BM188X::LRNLower>();
  pRegistry.emplace<BM188X::LeakyReluLower>();
  pRegistry.emplace<BM188X::SumLower>();
  pRegistry.emplace<BM188X::StoreLower>();
  pRegistry.emplace<BM188X::ScaleLower>();
  pRegistry.emplace<BM188X::ReluLower>();
}
