#ifndef CORTEXM_MAIN_FILE_HEADER_GEN_PASS_H
#define CORTEXM_MAIN_FILE_HEADER_GEN_PASS_H

#include "CortexmMeta.h"

#include <onnc/Core/CustomPass.h>

namespace onnc {
class TargetBackend;

class CortexmMainFileHeaderGenPass : public CustomPass<CortexmMainFileHeaderGenPass>
{
public:
  explicit CortexmMainFileHeaderGenPass(TargetBackend* pBackend, CortexmBackendMeta* pMeta);
  ReturnType runOnModule(Module& pModule) override;

private:
  TargetBackend*      m_pBackend;
  CortexmBackendMeta* m_pMeta;
};
} // namespace onnc

#endif
