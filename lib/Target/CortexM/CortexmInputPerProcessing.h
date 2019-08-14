#ifndef ONNC_CORTEXM_INPUT_PRE_REOCESSING_H
#define ONNC_CORTEXM_INPUT_PRE_REOCESSING_H

#include <onnc/Core/CustomPass.h>
#include "CortexMMeta.h"

namespace onnc{
  class TargetBackend;

  class CortexmInputPerProcessing : public CustomPass<CortexmInputPerProcessing>{
    public:
      explicit CortexmInputPerProcessing(TargetBackend *pBackend , CortexMBackendMeta *pMeta);
      ReturnType runOnModule(Module& pModule) override;
    private:
      TargetBackend *m_pBackend;
      CortexMBackendMeta *m_pMeta;
  };

}

#endif

