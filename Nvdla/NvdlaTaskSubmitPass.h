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

#include "NvdlaMeta.h"

namespace onnc {

class TargetBackend;

class NvdlaTaskSubmitPass : public ModulePass
{
public:
  static char ID;

public:
  NvdlaTaskSubmitPass(TargetBackend *pBackend,
                  NvdlaBackendMeta *pMeta);

  ReturnType runOnModule(Module& pModule) override;
  int submitEvent(int task_id, int event_type);
  int submitMemAllocAddress(int size, std::string blob_name);

private:

  TargetBackend *m_pBackend;
  NvdlaBackendMeta *m_pMeta;
};

// XXX: Experimental
NvdlaTaskSubmitPass *CreateNvdlaTaskSubmitPass(TargetBackend *pBackend,
                                          NvdlaBackendMeta *pMeta);

} // namespace of onnc

#endif
