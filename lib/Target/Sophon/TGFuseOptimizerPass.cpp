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
#include "TG.h"
#include "TGBackend.h"
#include <onnc/Core/CustomPass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Dump.h>
#include <onnc/Config/ONNX.h>

using namespace onnc;

namespace {

class ONNXFuseOpt : public CustomPass<ONNXFuseOpt>
{
public:
  ONNXFuseOpt(TGBackend *pTarget) : m_pTarget(pTarget) {}

  StringRef getPassName() const override { return "ONNXFuseOpt"; }
  
  Pass::ReturnType runOnModule(Module &pModule) override;

private:
  TGBackend *m_pTarget; // NOLINT
};

} // namespace

Pass::ReturnType ONNXFuseOpt::runOnModule(Module &pModule)
{
  auto TFO = m_pTarget->getFuseOptimizr();
  TFO->PrepareFuseOptimizer(pModule);
  xGraph *graph = pModule.getGraphIR().get();
  bool changed = TFO->FuseOptimization(graph, pModule.getSetId()[""]);
  return changed ? Pass::kModuleChanged : Pass::kModuleNoChanged;
}

ModulePass *onnc::createONNXFuseOptPass(TGBackend *pTarget)
{
  return new ONNXFuseOpt(pTarget);
}
