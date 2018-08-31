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
#include "AddLutTablePass.h"
#include "BM188x/BM188xTargetMemInfo.h"
#include "BM188x/BM188xTargetTransformInfo.h"
#include "BM188xEncodeInstsPass.h"
#include "BM188xFuseOptimizer.h"
#include "CodeEmitVisitor.h"
#include "GenRuntimeInfoPass.h"
#include "GenWeightPass.h"
#include "Lowers/AveragePoolLower.h"
#include "Lowers/BMScaleLower.h"
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
#include "Lowers/StoreLower.h"
#include "Lowers/SumLower.h"
#include "Lowers/TransposeLower.h"
#include "Lowers/UpsampleLower.h"
#include "TG.h"
#include <google/protobuf/text_format.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/IR/ONNCModulePrinter.h>
#include <onnc/Support/Casting.h>
#include <onnc/Support/OFStream.h>
#include <onnc/Transforms/BookONNXGraphs.h>
#include <onnc/Transforms/BuildInitializers.h>
#include <onnc/Transforms/BuildInputOperators.h>
#include <onnc/Transforms/BuildOutputOperators.h>
#include <onnc/Transforms/DeadNodeElimination.h>
#include <onnc/Transforms/RemoveTrainingNodes.h>
#include <onnc/Transforms/TensorSel.h>
#include <onnc/Transforms/TensorSel/LowerRegistry.h>
#include <onnc/Transforms/TensorSel/Standards/AddLower.h>
#include <onnc/Transforms/TensorSel/Standards/BatchNormalizationLower.h>
#include <onnc/Transforms/TensorSel/Standards/FlattenLower.h>
#include <onnc/Transforms/TensorSel/Standards/MulLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReshapeLower.h>
#include <onnc/Transforms/TensorSel/Standards/SoftmaxLower.h>
#include <onnc/Transforms/TensorSel/Standards/SplitLower.h>
#ifdef BMONNC_EXIST
#include <bmnetc/bmnetc.h>
#endif

using namespace onnc;

//===----------------------------------------------------------------------===//
// BM1880
//===----------------------------------------------------------------------===//
BM1880Backend::BM1880Backend(const TargetOptions &pOptions)
    : TGBackend(pOptions)
{
  m_pTTI = new BM188xTargetTransformInfo(this);
}

void BM1880Backend::addTensorSel(PassManager &pPM)
{
#ifdef BMONNC_EXIST
  bmnetc_pass_extention(this, pPM);
#endif

  bool dumpOptONNXModel = !options().optOnnxModel().empty();

  // target independent pass
  pPM.add(CreateRemoveTrainingNodesPass());
  // TODO refactoring, AddDummyWeightPass can be target indepedent pass
  if (options().shouldUseDummyWeight())
    pPM.add(createAddDummyWeightPass());
  pPM.add(CreateUpdateGraphOutputSizePass());
  pPM.add(CreateDeadNodeEliminationPass());

  // BM1880 customized Pass
  pPM.add(createPrepareCtablePass(this));

  // dump optimized onnx model pass need to
  // disable fuse opt
  if (!dumpOptONNXModel)
    pPM.add(createONNXFuseOptPass(this));

  if (options().shouldPrintBeforeTensorSel())
    pPM.add(createONNCModulePrinterPass());

  if (dumpOptONNXModel)
    pPM.add(createONNXDumpOptPass(this));

  pPM.add(CreateBookONNXGraphs());
  pPM.add(CreateBuildInitializers());
  pPM.add(CreateBuildInputOperators());
  pPM.add(CreateTensorSel(this));
  pPM.add(CreateBuildOutputOperators());
  pPM.add(BM188X::CreateAddLutTablePass(this));
#ifdef BMONNC_EXIST
  pPM.add(CreateNewQuantizePass(this));
#endif
  pPM.add(createUpdateCtablePass(this));

  return;
}

void BM1880Backend::addCodeEmit(PassManager &pPM, const Path &pOutputFile)
{
  static BM188X::CodeEmitVisitor ceVisitor(this);
  pPM.add(BM188X::CreateGenRuntimeInfoPass(this, pOutputFile));
  pPM.add(BM188X::CreateGenWeightPass(this, pOutputFile));
  pPM.add(BM188X::CreateEncodeInstsPass(this, &ceVisitor, pOutputFile.native()));
}

bool BM1880Backend::isNativeTensorType(xTensorProtoDataType pType)
{
  switch (pType) {
  case xValueType::kInt8:
  case xValueType::kInt16:
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
BM1880Backend::getLayerCtable(const std::string &pName) const
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
  pRegistry.emplace<onnc::AddLower>();
  pRegistry.emplace<BM188X::AveragePoolLower>();
  pRegistry.emplace<onnc::BatchNormalizationLower>();
  pRegistry.emplace<BM188X::BMScaleLower>();
  pRegistry.emplace<BM188X::ConcatLower>();
  pRegistry.emplace<BM188X::ConvLower>();
  pRegistry.emplace<onnc::FlattenLower>();
  pRegistry.emplace<BM188X::GemmLower>();
  pRegistry.emplace<BM188X::GlobalAveragePoolLower>();
  pRegistry.emplace<BM188X::LRNLower>();
  pRegistry.emplace<BM188X::LeakyReluLower>();
  pRegistry.emplace<BM188X::LoadLower>();
  pRegistry.emplace<BM188X::MaxPoolLower>();
  pRegistry.emplace<onnc::MulLower>();
  pRegistry.emplace<BM188X::PReluLower>();
  pRegistry.emplace<BM188X::PoolLower>();
  pRegistry.emplace<BM188X::ReluLower>();
  pRegistry.emplace<onnc::ReshapeLower>();
  pRegistry.emplace<onnc::SoftmaxLower>();
  pRegistry.emplace<onnc::SplitLower>();
  pRegistry.emplace<BM188X::StoreLower>();
  pRegistry.emplace<BM188X::SumLower>();
  pRegistry.emplace<BM188X::TransposeLower>();
  pRegistry.emplace<BM188X::UpsampleLower>();
}

std::shared_ptr<std::ostream> BM1880Backend::get_OSAsm() { return m_OSAsm; }
void BM1880Backend::set_OSAsm(std::shared_ptr<std::ostream> pOS)
{
  m_OSAsm = pOS;
}
