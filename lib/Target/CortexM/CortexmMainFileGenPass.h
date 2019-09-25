#ifndef ONNC_CORTEXM_MAIN_FILE_GEN_PASS_H
#define ONNC_CORTEXM_MAIN_FILE_GEN_PASS_H

#include "CortexmMeta.h"
#include <onnc/Core/CustomPass.h>

namespace onnc {
class TargetBackend;

class CortexmMainFileGenPass : public CustomPass<CortexmMainFileGenPass> {
public:
  explicit CortexmMainFileGenPass(TargetBackend* pBackend,
                              CortexmBackendMeta* pMeta);
  ReturnType runOnModule(Module& pModule) override;

private:
  TargetBackend* m_pBackend;
  CortexmBackendMeta* m_pMeta;
};
}

#endif
