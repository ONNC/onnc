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
#ifndef ONNC_TARGET_BM168X_BACKEND_H
#define ONNC_TARGET_BM168X_BACKEND_H
#include "TGBackend.h"
#include <onnc/Config/ONNX.h>
#include <memory>
#include <string>

namespace onnc {

class TargetLowering;
class TGCodeEmitter;

class BM1680Backend : public TGBackend
{
public:
  BM1680Backend(TGBackend::Instructions& pInsns, const TargetOptions &pOptions);

  ~BM1680Backend() override;

  bool isNativeTensorType(xTensorProtoDataType pType) override;

  void addTensorSel(PassManager &pPM) override;

  std::string getBackendName() override { return "BM1680Backend"; }

  std::unique_ptr<TGFuseOptimizer> getFuseOptimizr() override;
};

class BM1682Backend : public TGBackend
{
public:
  BM1682Backend(TGBackend::Instructions& pInsns, const TargetOptions &pOptions);

  ~BM1682Backend() override;

  void addTensorSel(PassManager &pPM) override;

  bool isNativeTensorType(xTensorProtoDataType pType) override;

  std::string getBackendName() override { return "BM1682Backend"; }

  std::unique_ptr<TGFuseOptimizer> getFuseOptimizr() override;
};

} // namespace onnc

#endif // BM168X_BACKEND_H
