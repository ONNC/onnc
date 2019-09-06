//===- InterpreterPass.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_NVDLAM_TASKSUBMIT_PASS_H
#define ONNC_NVDLAM_TASKSUBMIT_PASS_H

#include "NvDlaMeta.h"
#include "Version.h"

#include <onnc/Core/CustomPass.h>

namespace onnc {
class TargetBackend;

class NvDlaTaskSubmitPass : public CustomPass<NvDlaTaskSubmitPass>
{
public:
  NvDlaTaskSubmitPass(NvDlaBackendMeta* pMeta, Version pDlaVersion, Version pEmuVersion);

  ReturnType runOnModule(Module& pModule) override;
  int        submitEvent(int task_id, int event_type);
  int        submitMemAllocAddress(int size, std::string blob_name);

private:
  NvDlaBackendMeta* m_pMeta;
  const Version     m_DlaVersion;
  const Version     m_EmuVersion;
};

} // namespace onnc

#endif
