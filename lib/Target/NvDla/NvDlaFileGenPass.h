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

#include <onnc/Core/CustomPass.h>

namespace onnc {
class TargetBackend;

class NvDlaFileGenPass : public CustomPass<NvDlaFileGenPass>
{
public:
  explicit NvDlaFileGenPass(NvDlaBackendMeta* pMeta);

  ReturnType runOnModule(Module& pModule) override;

private:
  NvDlaBackendMeta* m_pMeta;
};

} // namespace onnc

#endif
