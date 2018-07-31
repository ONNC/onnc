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
#include "TargetLowering.h"
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnx/common/ir.h>

using namespace onnc;

namespace {

class TargetISel : public ModulePass
{
public:
  static char ID;

public:
  TargetISel(TGBackend *pBackend) : ModulePass(ID), m_pTarget(pBackend) {}

  StringRef getPassName() const override { return "TargetISel"; }
  
  Pass::ReturnType runOnModule(::onnc::Module &pModule) override
  {
    TargetLowering *TLI = m_pTarget->getTargetLowering();
    TLI->PrepareISelLowering(pModule);
    ::onnx::Graph *graph = pModule.getGraphIR().get();
    TLI->ISelLowering(graph, m_pTarget->getInsts());
    return Pass::kModuleNoChanged;
  }

private:
  TGBackend *m_pTarget; // NOLINT
};

} // anonymous namespace

char TargetISel::ID = 0;

ModulePass *onnc::createTargetLoweringPass(TGBackend *pTarget)
{
  return new TargetISel(pTarget);
}
