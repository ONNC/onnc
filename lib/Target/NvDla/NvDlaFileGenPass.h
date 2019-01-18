//===- InterpreterPass.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_NVDLAM_FILEGEN_PASS_H
#define ONNC_NVDLAM_FILEGEN_PASS_H

#include <onnc/Core/CustomPass.h>

#include "NvDlaMeta.h"

namespace onnc {

class TargetBackend;

class NvDlaFileGenPass : public CustomPass<NvDlaFileGenPass>
{
public:
  NvDlaFileGenPass(TargetBackend *pBackend,
                  NvDlaBackendMeta *pMeta);

  ReturnType runOnModule(Module& pModule) override;

private:

  TargetBackend *m_pBackend;
  NvDlaBackendMeta *m_pMeta;
};

// XXX: Experimental
NvDlaFileGenPass *CreateNvDlaFileGenPass(TargetBackend *pBackend,
                                          NvDlaBackendMeta *pMeta);

} // namespace of onnc

#endif
