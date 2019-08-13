#ifndef CORTEXM_HERDER_GEN_PASS_H
#define CORTEXM_HERDER_GEN_PASS_H

#include <onnc/Core/ModulePass.h>
#include <onnc/Core/CustomPass.h>
#include "CortexMMeta.h"

namespace onnc{
  class TargetBackend;

  class CortexmHeaderFileGenPass : public ModulePass<CortexmHeaderFileGenPass>{
    public:
      static char ID;
      CortexmHeaderFileGenPass(TargetBackend *pBackend , CortexMBackendMeta *pMeta);
      ReturnType runOnModule(Module& pModule) override;
    private:
      TargetBackend *m_pBackend;
      CortexMBackendMeta *m_pMeta;
  };

  CortexmHeaderFileGenPass *CreateCortemHeaderFileGenPass(TargetBackend *pBackend , CortexMBackendMeta *pMeta);

}

#endif

