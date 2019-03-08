//===- InterpreterPass.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_NVDLAM_TASKSUBMIT_PASS_H
#define ONNC_NVDLAM_TASKSUBMIT_PASS_H

#include <onnc/Core/ModulePass.h>

#include "NvDlaMeta.h"

namespace onnc {

class TargetBackend;

class NvDlaTaskSubmitPass : public ModulePass
{
public:
  static char ID;

public:
  NvDlaTaskSubmitPass(TargetBackend *pBackend,
                  NvDlaBackendMeta *pMeta);

  ReturnType runOnModule(Module& pModule) override;
  int submitEvent(int task_id, int event_type);
  int submitMemAllocAddress(int size, std::string blob_name);

private:

  TargetBackend *m_pBackend;
  NvDlaBackendMeta *m_pMeta;
};

// XXX: Experimental
NvDlaTaskSubmitPass *CreateNvDlaTaskSubmitPass(TargetBackend *pBackend,
                                          NvDlaBackendMeta *pMeta);

} // namespace of onnc

#endif
