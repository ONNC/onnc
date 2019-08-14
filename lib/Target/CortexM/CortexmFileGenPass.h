#ifndef ONNC_CORTEXM_FILEGEN_PASS_H
#define ONNC_CORTEXM_FILEGEN_PASS_H

#include <onnc/Core/CustomPass.h>
#include "CortexmMeta.h"

namespace onnc{
  class TargetBackend;

  class CortexmFileGenPass : public CustomPass<CortexmFileGenPass>
  {
    public:
      explicit CortexmFileGenPass(TargetBackend *pBackend , CortexmBackendMeta *pMeta);
      ReturnType runOnModule(Module& pModule) override;
    private:
      TargetBackend *m_pBackend;
      CortexmBackendMeta *m_pMeta;
  };

}

#endif

