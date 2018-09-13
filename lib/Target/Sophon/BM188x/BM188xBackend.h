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
#ifndef BM188X_BACKEND_H
#define BM188X_BACKEND_H
#include "TGBackend.h"
#include <memory>
#include <onnc/Target/Sophon/BM188x/common_calibration2.pb.h>
#include <onnc/Config/ONNX.h>
#include <ostream>
#include <string>

namespace onnc {

class TGCodeEmitter;
class TargetTransformInfo;

class BM1880Backend : public TGBackend
{
public:
  using LayerCtable = tg::bm1880::LayerCalibrationParameter;

public:
  BM1880Backend(const TargetOptions &pOptions);

  ~BM1880Backend() override = default;

  /// override TensorSel stage.
  void addTensorSel(PassManager &pPM) override;

  void addCodeEmit(PassManager &pPM, const Path &pOutputFile) override;

  bool isNativeTensorType(xTensorProtoDataType pType) override;

  std::string getBackendName() override
  {
    return std::string("BM1880Backend");
  };

  std::string getCtableName() override { return "bm1880_ctable"; }

  void setCtableProto(const std::string &pTextString) override;

  LayerCtable* getMutableLayerCtable(const std::string &pName);

  const LayerCtable *getLayerCtable(const std::string &pName) const;

  const TargetTransformInfo *getTTI() const override { return m_pTTI; }

  std::unique_ptr<TGFuseOptimizer> getFuseOptimizr() override;

  /// register lowers for TensorSel.
  void RegisterLowers(LowerRegistry& pRegistry) const override;

  tg::bm1880::NetCalibrationParameter &getBackendCtable()
  {
    return m_NetCtableParam;
  }
  std::shared_ptr<std::ostream> get_OSAsm();
  void set_OSAsm(std::shared_ptr<std::ostream> pOS);

private:
  std::shared_ptr<std::ostream> m_OSAsm;
  tg::bm1880::NetCalibrationParameter m_NetCtableParam;
  TargetTransformInfo *m_pTTI; // NOLINT
};

//===----------------------------------------------------------------------===//
// Factory Methods
//===----------------------------------------------------------------------===//
ModulePass *createPrepareCtablePass(BM1880Backend *pBackend);
ModulePass *createUpdateCtablePass(BM1880Backend *pBackend);
ModulePass *CreateNewQuantizePass(BM1880Backend *pBackend);
ModulePass *createAddDummyWeightPass();
ModulePass *createONNXDumpOptPass(BM1880Backend *pBackend);
ModulePass *createONNXDumpQuantizedPass(BM1880Backend *pBackend);

} // namespace onnc

#endif // BM188X_BACKEND_H
