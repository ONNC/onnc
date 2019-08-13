#ifndef CORTEXM_MAIN_FILE_HEADER_GEN_PASS
#define CORTEXM_MAIN_FILE_HEADER_GEN_PASS

#include <onnc/Core/ModulePass.h>
#include "CortexMMeta.h"

namespace onnc{
  class TargetBackend;

  class CortexmMainFileHeaderGenPass : public ModulePass<CortexmMainFileHeaderGenPass>{
    public:
      static char ID;
      
      CortexmMainFileHeaderGenPass(TargetBackend *pBackend , CortexMBackendMeta *pMeta);
      ReturnType runOnModule(Module& pModule) override;
    private:
      TargetBackend *m_pBackend;
      CortexMBackendMeta *m_pMeta;
  };

  CortexmMainFileHeaderGenPass *CreateCortexmMainFileHeaderGenPass(TargetBackend *pBackend , CortexMBackendMeta *pMeta);

}


#endif
