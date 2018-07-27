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
#include "BM188xBackend.h"
#include "BM188xComputeOperator.h"
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnx/common/ir.h>

using namespace onnc;

namespace {

class UpdateCtablePass : public ModulePass
{
public:
  static char ID;

public:
  UpdateCtablePass(BM1880Backend *pBackend)
      : ModulePass(ID), m_pBackend(pBackend)
  {
  }

  Pass::ReturnType runOnModule(Module &pModule) override;

private:
  BM1880Backend *m_pBackend; // NOLINT
};

} // namespace

Pass::ReturnType UpdateCtablePass::runOnModule(Module &pModule)
{
  std::vector<std::unique_ptr<ComputeOperator2> > &instList =
      m_pBackend->getInsts();
  for (auto &i : instList) {
    const auto *layerCtable = m_pBackend->getLayerCtable(i->getLayerName());
    static_cast<BM188xComputeOperator *>(i.get())->update(layerCtable);
  }
  return Pass::kModuleChanged;
}

char UpdateCtablePass::ID = 0;

ModulePass *onnc::createUpdateCtablePass(BM1880Backend *pBackend)
{
  return new UpdateCtablePass(pBackend);
}
