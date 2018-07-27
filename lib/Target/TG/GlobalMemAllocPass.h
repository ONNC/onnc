//===- GlobalMemAllocPass.h -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TG_GLOBAL_MEMORY_ALLOCATION_PASS_H
#define ONNC_TARGET_TG_GLOBAL_MEMORY_ALLOCATION_PASS_H
#include "TG.h"
#include "TGBackend.h"
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>

namespace onnc {
namespace BM188X {

class GlobalMemAlloc : public ModulePass
{
public:
  static char ID;

public:
  GlobalMemAlloc(TGBackend *pTarget);

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;

private:
  TGBackend *m_pTarget; // NOLINT
};

} // namespace BM188X

ModulePass *CreateGlobalMemAllocPass(TGBackend *pTarget);

} // namespace onnc

#endif
