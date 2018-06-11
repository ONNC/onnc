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
#include <google/protobuf/text_format.h>

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

void BM1880Backend::setCtableProto(const std::string &pTextString)
{
  ::google::protobuf::TextFormat::ParseFromString(pTextString,
                                                  &m_NetCtableParam);
}

const tg::bm1880::LayerCalibrationParameter &
BM1880Backend::getCtableLayerParam(std::string &pName)
{
  for (int i = 0; i < m_NetCtableParam.layer_size(); i++) {
    const tg::bm1880::LayerCalibrationParameter &layer =
        m_NetCtableParam.layer(i);
    if (layer.name() == pName) {
      return layer;
    }
  }
  assert(0);
}
