#ifndef ONNC_CORTEXM_FILEGEN_PASS_H
#define ONNC_CORTEXM_FILEGEN_PASS_H

#include <onnc/Core/CustomPass.h>
#include "CortexMMeta.h"

namespace onnc{
  class TargetBackend;

  class CortexmFileGenPass : public CustomPass<CortexmFileGenPass>
  {
    public:
      explicit CortexmFileGenPass(TargetBackend *pBackend , CortexMBackendMeta *pMeta);
      ReturnType runOnModule(Module& pModule) override;
    private:
      TargetBackend *m_pBackend;
      CortexMBackendMeta *m_pMeta;
  };

}

#endif

