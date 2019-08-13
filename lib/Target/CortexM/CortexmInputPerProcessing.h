#ifndef ONNC_CORTEXM_INPUT_PRE_REOCESSING_H
#define ONNC_CORTEXM_INPUT_PRE_REOCESSING_H

#include <onnc/Core/ModulePass.h>
#include "CortexMMeta.h"

namespace onnc{
  class TargetBackend;

  class CortexmInputPerProcessing : public ModulePass<CortexmInputPerProcessing>{
    public:
      static char ID;
      CortexmInputPerProcessing(TargetBackend *pBackend , CortexMBackendMeta *pMeta);
      ReturnType runOnModule(Module& pModule) override;
    private:
      TargetBackend *m_pBackend;
      CortexMBackendMeta *m_pMeta;
  };

  CortexmInputPerProcessing *CreateCortexmInputPerProcessing(TargetBackend *pBackend , CortexMBackendMeta *pMeta);

}

#endif

