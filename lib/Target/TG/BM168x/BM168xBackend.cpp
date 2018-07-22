//===- BM168xBackend.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "BM168xBackend.h"
#include "BM168xCodeEmitter.h"
#include "BM168xFuseOptimizer.h"
#include "BM168xISelLowering.h"
#ifdef BMNETC_EXIST
#include <bmnetc/bmnetc.h>
#endif

using namespace onnc;

//===----------------------------------------------------------------------===//
// BM1680
//===----------------------------------------------------------------------===//
BM1680Backend::BM1680Backend(const TargetOptions &pOptions)
    : TGBackend(new BM168xTargetLowering(this), new BM168xCodeEmitter(this),
                pOptions)
{
}

BM1680Backend::~BM1680Backend() = default;

void BM1680Backend::addTensorSel(PassManager &pPM)
{
#ifdef BMNETC_EXIST
  bmnetc_pass_extention(backend, pm);
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
BM1682Backend::BM1682Backend(const TargetOptions &pOptions)
    : TGBackend(new BM168xTargetLowering(this), new BM168xCodeEmitter(this),
                pOptions)
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
#ifdef BMNETC_EXIST
  bmnetc_pass_extention(backend, pm);
#endif
}

