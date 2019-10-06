//===- InterpreterPass.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_NVDLAM_FILEGEN_PASS_H
#define ONNC_NVDLAM_FILEGEN_PASS_H

#include "NvDlaMeta.h"
#include "Version.h"

#include <onnc/Core/CustomPass.h>

#include <cstdint>

namespace onnc {
class TargetBackend;

class NvDlaFileGenPass : public CustomPass<NvDlaFileGenPass>
{
public:
  NvDlaFileGenPass(NvDlaBackendMeta* pMeta, Version pLoadableVersion);

  ReturnType runOnModule(Module& pModule) override;

private:
  NvDlaBackendMeta* m_pMeta;
  const Version     m_LoadableVersion;
};

} // namespace onnc

#endif
