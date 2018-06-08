//===- BM188xBackend.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef BM188X_BACKEND_H
#define BM188X_BACKEND_H
#include "TGBackend.h"
#include "common_calibration.pb.h"
#include <memory>
#include <onnx/common/ir.h>
#include <string>

namespace onnc {

class TGCodeEmitter;

class BM1880Backend : public TGBackend
{
public:
  BM1880Backend(const TargetOptions &pOptions);
  virtual ~BM1880Backend() = default;
  bool isNativeTensorType(::onnx::TensorProto_DataType pType) override;
  std::string getBackendName() override { return "BM1880Backend"; };
  std::string getCtableName() override { return "bm1880_ctable"; }
  void setCtable(const std::string &pTextString);
  const LayerCalibrationParameter &getCtableLayerParam(std::string &pName);

private:
  NetCalibrationParameter m_NetCtableParam;
};

} // namespace onnc

#endif // BM188X_BACKEND_H
