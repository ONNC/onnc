//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#include "BM188xBackend.h"
#include "BM188x/BM188xTargetMemInfo.h"
#include "BM188x/BM188xTargetTransformInfo.h"
#include "BM188xCodeEmitter.h"
#include "BM188xFuseOptimizer.h"
#include "BM188xISelLowering.h"
#include "Lowers/AveragePoolLower.h"
#include "Lowers/ConcatLower.h"
#include "Lowers/ConvLower.h"
#include "Lowers/GemmLower.h"
#include "Lowers/GlobalAveragePoolLower.h"
#include "Lowers/LRNLower.h"
#include "Lowers/LeakyReluLower.h"
#include "Lowers/LoadLower.h"
#include "Lowers/MaxPoolLower.h"
#include "Lowers/PReluLower.h"
#include "Lowers/PoolLower.h"
#include "Lowers/ReluLower.h"
#include "Lowers/ScaleLower.h"
#include "Lowers/StoreLower.h"
#include "Lowers/SumLower.h"
#include "Lowers/TransposeLower.h"
#include "Lowers/UpsampleLower.h"
#include "CodeEmitVisitor.h"
#include "EncodeInstructionsPass.h"
#include "TG.h"
#include <google/protobuf/text_format.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/IR/ONNCModulePrinter.h>
#include <onnc/Transforms/BookONNXGraphs.h>
#include <onnc/Transforms/BuildInputOperators.h>
#include <onnc/Transforms/BuildInitializers.h>
#include <onnc/Transforms/BuildOutputOperators.h>
#include <onnc/Transforms/DeadNodeElimination.h>
#include <onnc/Transforms/RemoveTrainingNodes.h>
#include <onnc/Transforms/TensorSel.h>
#include <onnc/Transforms/TensorSel/LowerRegistry.h>
#ifdef BMONNC_EXIST
#include <bmnetc/bmnetc.h>
#endif

using namespace onnc;

//===----------------------------------------------------------------------===//
// BM1880
//===----------------------------------------------------------------------===//
BM1880Backend::BM1880Backend(Instructions& pInsn, const TargetOptions &pOptions)
    : TGBackend(new BM188xISelLowering(this),
                new BM188xCodeEmitter(this, pInsn), pInsn, pOptions)
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
  if (options().shouldUseDummyWeight())
    pPM.add(CreateAddDummyWeightPass());
  pPM.add(CreateUpdateGraphOutputSizePass());

  // BM1880 customized Pass
  pPM.add(createPrepareCtablePass(this));
  pPM.add(createONNXFuseOptPass(this));
  if (options().shouldPrintBeforeTensorSel())
    pPM.add(createONNCModulePrinterPass());

  if (getTargetLower() == nullptr) {
    pPM.add(createTargetLoweringPass(this));
  } else {
    pPM.add(getTargetLower()(this));
  }

  pPM.add(createUpdateCtablePass(this));
  pPM.add(CreateDeadNodeEliminationPass());
  pPM.add(CreateBookONNXGraphs());
  pPM.add(CreateBuildInitializers());
  pPM.add(CreateBuildInputOperators());
  pPM.add(CreateTensorSel(this));
  pPM.add(CreateBuildOutputOperators());

  return;
}

void BM1880Backend::addCodeEmit(PassManager &pPM, const Path &pOutputFile)
{
  static BM188X::CodeEmitVisitor ceVisitor(this);
  pPM.add(CreateEncodeInstructionsPass(&ceVisitor));
  TGBackend::addCodeEmit(pPM, pOutputFile);
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

BM1880Backend::LayerCtable *
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

const BM1880Backend::LayerCtable *
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
  pRegistry.emplace<BM188X::ConcatLower>();
  pRegistry.emplace<BM188X::ConvLower>();
  pRegistry.emplace<BM188X::GemmLower>();
  pRegistry.emplace<BM188X::GlobalAveragePoolLower>();
  pRegistry.emplace<BM188X::LRNLower>();
  pRegistry.emplace<BM188X::LeakyReluLower>();
  pRegistry.emplace<BM188X::LoadLower>();
  pRegistry.emplace<BM188X::MaxPoolLower>();
  pRegistry.emplace<BM188X::PReluLower>();
  pRegistry.emplace<BM188X::PoolLower>();
  pRegistry.emplace<BM188X::ReluLower>();
  pRegistry.emplace<BM188X::ScaleLower>();
  pRegistry.emplace<BM188X::StoreLower>();
  pRegistry.emplace<BM188X::SumLower>();
  pRegistry.emplace<BM188X::TransposeLower>();
  pRegistry.emplace<BM188X::UpsampleLower>();
}
