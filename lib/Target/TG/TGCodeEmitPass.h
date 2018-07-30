//===- TGCodeEmitPass.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TG_CODE_EMIT_PASS_H
#define ONNC_TARGET_TG_CODE_EMIT_PASS_H
#include <onnc/Core/ModulePass.h>
#include <string>

namespace onnc {

class TGBackend;

namespace TG {

class TGCodeEmit : public ModulePass
{
public:
  static char ID;

public:
  TGCodeEmit(TGBackend *pTarget, const std::string &pOutputFilename);

  Pass::ReturnType runOnModule(Module &pModule) override;

private:
  TGBackend *m_Target;
  std::string m_OutputFilename;
};

} // namespace TG

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
ModulePass *CreateTGCodeEmitPass(TGBackend *pTarget,
                                 const std::string &pOutputFilename);

} // namespace onnc

#endif
