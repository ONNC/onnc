#ifndef CORTEXM_HERDER_GEN_PASS_H
#define CORTEXM_HERDER_GEN_PASS_H

#include <onnc/Core/CustomPass.h>
#include "CortexMMeta.h"

namespace onnc{
  class TargetBackend;

  class CortexmHeaderFileGenPass : public CustomPass<CortexmHeaderFileGenPass>{
    public:
      explicit CortexmHeaderFileGenPass(TargetBackend *pBackend , CortexMBackendMeta *pMeta);
      ReturnType runOnModule(Module& pModule) override;
    private:
      TargetBackend *m_pBackend;
      CortexMBackendMeta *m_pMeta;
  };

}

#endif

