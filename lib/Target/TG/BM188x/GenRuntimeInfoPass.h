//===- GenRuntimeInfoPass.h -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TG_GEN_RUNTIME_INFO_PASS_H
#define ONNC_TARGET_TG_GEN_RUNTIME_INFO_PASS_H
#include <onnc/Core/ModulePass.h>
#include <onnc/Support/Path.h>
#include "../TGBackend.h"

namespace onnc {
namespace BM188X {

class GenRuntimeInfoPass : public ModulePass
{
public:
  static char ID;

public:
  GenRuntimeInfoPass(TGBackend* pBackend, const Path &pOutFile);

  Pass::ReturnType runOnModule(Module &pModule) override;

private:
  TGBackend *backend() { return m_pBackend; }

  const TGBackend *backend() const { return m_pBackend; }

private:
  TGBackend *m_pBackend;
  Path m_OutFile;
};

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
ModulePass* CreateGenRuntimeInfoPass(TGBackend* pBackend, const Path& pOutFile);

} // namespace BM188X
} // namespace onnc

#endif
