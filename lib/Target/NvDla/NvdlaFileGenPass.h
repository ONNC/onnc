//===- InterpreterPass.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_NVDLAM_FILEGEN_PASS_H
#define ONNC_NVDLAM_FILEGEN_PASS_H

#include <onnc/Core/ModulePass.h>

#include "NvdlaMeta.h"

namespace onnc {

class TargetBackend;

class NvdlaFileGenPass : public ModulePass
{
public:
  static char ID;

public:
  NvdlaFileGenPass(TargetBackend *pBackend,
                  NvdlaBackendMeta *pMeta);

  ReturnType runOnModule(Module& pModule) override;

private:

  TargetBackend *m_pBackend;
  NvdlaBackendMeta *m_pMeta;
};

// XXX: Experimental
NvdlaFileGenPass *CreateNvdlaFileGenPass(TargetBackend *pBackend,
                                          NvdlaBackendMeta *pMeta);

} // namespace of onnc

#endif
