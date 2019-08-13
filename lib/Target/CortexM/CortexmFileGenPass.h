#ifndef ONNC_CORTEXM_FILEGEN_PASS_H
#define ONNC_CORTEXM_FILEGEN_PASS_H

#include <onnc/Core/ModulePass.h>
#include "CortexMMeta.h"

namespace onnc{
  class TargetBackend;

  class CortexmFileGenPass : public ModulePass<CortexmFileGenPass>
  {
    public:
      static char ID;
      
      CortexmFileGenPass(TargetBackend *pBackend , CortexMBackendMeta *pMeta);
      ReturnType runOnModule(Module& pModule) override;
    private:
      TargetBackend *m_pBackend;
      CortexMBackendMeta *m_pMeta;
  };

  CortexmFileGenPass *CreateCortexmFileGenPass(TargetBackend *pBackend , CortexMBackendMeta *pMeta);

}

#endif

