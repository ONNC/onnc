//===- BM1880Backend.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "BM188xBackend.h"
#include "BM188xCodeEmitter.h"
#include "BM188xISelLowering.h"

using namespace onnc;

// BM1880
BM1880Backend::BM1880Backend(const TargetOptions &pOptions)
    : TGBackend(new BM188xISelLowering(this), new BM188xCodeEmitter(this),
                pOptions)
{
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
