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
#include "BM168xBackend.h"
#include "BM168xCodeEmitter.h"
#include "BM168xFuseOptimizer.h"
#include "BM168xISelLowering.h"
#ifdef BMONNC_EXIST
#include <bmnetc/bmnetc.h>
#endif

using namespace onnc;

//===----------------------------------------------------------------------===//
// BM1680
//===----------------------------------------------------------------------===//
BM1680Backend::BM1680Backend(Instructions& pIns, const TargetOptions &pOptions)
    : TGBackend(new BM168xTargetLowering(this), new BM168xCodeEmitter(this),
                pIns, pOptions)
{
}

BM1680Backend::~BM1680Backend() = default;

void BM1680Backend::addTensorSel(PassManager &pPM)
{
#ifdef BMONNC_EXIST
  bmnetc_pass_extention(this, pPM);
#endif
}

bool BM1680Backend::isNativeTensorType(::onnx::TensorProto_DataType pType)
{
  if (pType == ::onnx::TensorProto_DataType_FLOAT) {
    return true;
  }
  return false;
}
std::unique_ptr<TGFuseOptimizer> BM1680Backend::getFuseOptimizr()
{
  return std::make_unique<BM168xFuseOptimizer>(this);
}

//===----------------------------------------------------------------------===//
// BM1682
//===----------------------------------------------------------------------===//
BM1682Backend::BM1682Backend(Instructions& pIns, const TargetOptions &pOptions)
    : TGBackend(new BM168xTargetLowering(this), new BM168xCodeEmitter(this),
                pIns, pOptions)
{
}

BM1682Backend::~BM1682Backend() = default;

bool BM1682Backend::isNativeTensorType(::onnx::TensorProto_DataType pType)
{
  if (pType == ::onnx::TensorProto_DataType_FLOAT) {
    return true;
  }
  return false;
}
std::unique_ptr<TGFuseOptimizer> BM1682Backend::getFuseOptimizr()
{
  return std::make_unique<BM168xFuseOptimizer>(this);
}

void BM1682Backend::addTensorSel(PassManager &pPM)
{
#ifdef BMONNC_EXIST
  bmnetc_pass_extention(this, pPM);
#endif
}
