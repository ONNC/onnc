//===- BM168xBackend.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "BM168xBackend.h"
#include "BM168xCodeEmitter.h"
#include "BM168xISelLowering.h"
using namespace onnc;

// BM1680
BM1680Backend::BM1680Backend(const TargetOptions &pOptions)
    : TGBackend(new BM168xTargetLowering(this), new BM168xCodeEmitter(this),
                pOptions)
{
}

BM1680Backend::~BM1680Backend() = default;

bool BM1680Backend::isNativeTensorType(::onnx::TensorProto_DataType pType)
{
  if (pType == ::onnx::TensorProto_DataType_FLOAT) {
    return true;
  }
  return false;
}

// BM1682
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
