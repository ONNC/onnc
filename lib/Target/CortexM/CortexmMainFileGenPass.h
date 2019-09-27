#ifndef ONNC_CORTEXM_MAIN_FILE_GEN_PASS_H
#define ONNC_CORTEXM_MAIN_FILE_GEN_PASS_H

#include "CortexmMeta.h"

#include <onnc/Core/CustomPass.h>

namespace onnc {
class TargetBackend;

class CortexmMainFileGenPass : public CustomPass<CortexmMainFileGenPass>
{
public:
  explicit CortexmMainFileGenPass(const CortexmBackendMeta& pMeta);
  ReturnType runOnModule(Module& pModule) override;

private:
  const CortexmBackendMeta& m_pMeta;
};
} // namespace onnc

#endif
