//===- BM1880Backend.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "BM1880Backend.h"
#include "BM1880CodeEmitter.h"
#include "BM1880ISelLowering.h"

using namespace onnc;

// BM1880
BM1880Backend::BM1880Backend(const TargetOptions &pOptions)
    : TGBackend(new BM1880ISelLowering(this), new BM1880CodeEmitter(this),
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
