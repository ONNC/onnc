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
#include "TGBackend.h"
#include "BM168x/BM168xBackend.h"
#include "BM188x/BM188xBackend.h"
#include "BuildMemOpndPass.h"
#include "LinearScanAllocPass.h"
#include "TG.h"
#include "TargetInfo/TGTargetInfo.h"
#include <algorithm>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/IR/ONNCModulePrinter.h>
#include <onnc/Target/TargetRegistry.h>
#include <onnc/Transforms/RemoveTrainingNodes.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// TGBackend
//===----------------------------------------------------------------------===//
TGBackend::TGBackend(const TargetOptions &pOptions) : DLATargetBackend(pOptions)
{
  m_ReplaceTargetLower = nullptr;
}

void TGBackend::addTensorSel(PassManager &pPM)
{
  // TODO
  assert(0);
  return;
}

void TGBackend::addMemAlloc(PassManager &pPM)
{
  pPM.add(CreateBuildMemOpndPass());
  pPM.add(CreateLinearScanAllocPass(this));
}

void TGBackend::addCodeEmit(PassManager &pPM, const Path &pOutput)
{
  // TODO
  assert(0);
}

bool TGBackend::isNativeTensorType(xTensorProtoDataType pType)
{
  return true;
}

size_t TGBackend::sizeOfTensorType(xTensorProtoDataType pType)
{
  if (!isNativeTensorType(pType)) {
    std::cerr << "unsupported pType " << TensorProto_DataType_Name(pType)
              << " in " << getBackendName() << "\n";
    return 0;
  }
  switch (pType) {
  case xValueType::kFloat:
    return sizeof(float);
  case xValueType::kInt8:
    return sizeof(int8_t);
  case xValueType::kInt16:
    return sizeof(int16_t);
  default:
    assert(0 && "unimplemented size of pType!");
  }
  return 0;
}

std::string TGBackend::getCtable(const Module &pModule)
{
  auto &meta_data = pModule.getMetaData();
  if (meta_data.find(getCtableName()) == meta_data.end())
    return std::string();
  return meta_data.at(getCtableName());
}

void TGBackend::setCtableProto(const std::string &pTextString)
{
  if (getCtableName() != "") {
    // implemnt this function when target has ctable
    assert(0);
  }
}

onnc::ComputeMemOperand* TGBackend::getMemOpndByValue(const onnc::Value* pVal)
{
  auto it = m_ValMemOpndMap.find(pVal);
  return it != m_ValMemOpndMap.end() ? it->second : nullptr;
}

//===----------------------------------------------------------------------===//
// Non member functions
//===----------------------------------------------------------------------===//
TargetBackend *CreateTGBM1680Backend(const TargetOptions &pOptions)
{
  return new BM1680Backend(pOptions);
}

TargetBackend *CreateTGBM1682Backend(const TargetOptions &pOptions)
{
  return new BM1682Backend(pOptions);
}

TargetBackend *CreateTGBM1880Backend(const TargetOptions &pOptions)
{
  return new BM1880Backend(pOptions);
}

extern "C" void InitializeSophonONNCBackend()
{
  onnc::TargetRegistry::RegisterTargetBackend(getTheTGBM1680Target(),
                                              CreateTGBM1680Backend);
  onnc::TargetRegistry::RegisterTargetBackend(getTheTGBM1682Target(),
                                              CreateTGBM1682Backend);
  onnc::TargetRegistry::RegisterTargetBackend(getTheTGBM1880Target(),
                                              CreateTGBM1880Backend);
}
