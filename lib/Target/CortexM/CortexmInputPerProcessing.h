#ifndef ONNC_CORTEXM_INPUT_PRE_REOCESSING_H
#define ONNC_CORTEXM_INPUT_PRE_REOCESSING_H

#include <onnc/Core/CustomPass.h>
#include "CortexmMeta.h"

namespace onnc{
  class TargetBackend;

  class CortexmInputPerProcessing : public CustomPass<CortexmInputPerProcessing>{
    public:
      explicit CortexmInputPerProcessing(TargetBackend *pBackend , CortexmBackendMeta *pMeta);
      ReturnType runOnModule(Module& pModule) override;
    private:
      TargetBackend *m_pBackend;
      CortexmBackendMeta *m_pMeta;
  };

}

#endif

