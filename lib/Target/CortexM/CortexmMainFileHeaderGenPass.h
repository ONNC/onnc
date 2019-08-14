#ifndef CORTEXM_MAIN_FILE_HEADER_GEN_PASS
#define CORTEXM_MAIN_FILE_HEADER_GEN_PASS

#include <onnc/Core/CustomPass.h>
#include "CortexMMeta.h"

namespace onnc{
  class TargetBackend;

  class CortexmMainFileHeaderGenPass : public CustomPass<CortexmMainFileHeaderGenPass>{
    public:
      explicit CortexmMainFileHeaderGenPass(TargetBackend *pBackend , CortexMBackendMeta *pMeta);
      ReturnType runOnModule(Module& pModule) override;
    private:
      TargetBackend *m_pBackend;
      CortexMBackendMeta *m_pMeta;
  };

}


#endif
