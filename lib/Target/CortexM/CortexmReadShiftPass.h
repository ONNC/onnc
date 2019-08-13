#ifndef CORTEXM_READ_SHIFT_PASS
#define CORTEXM_READ_SHIFT_PASS

#include <onnc/Core/ModulePass.h>
#include "CortexMMeta.h"

namespace onnc{
  class TargetBackend;

  class CortexmReadShiftPass : public ModulePass<CortexmReadShiftPass>{
    public:
      static char ID;
      
      struct shift_list* save_shift = (shift_list*)malloc(sizeof(shift_list));

      int first = 0;

      CortexmReadShiftPass(TargetBackend *pBackend , CortexMBackendMeta *pMeta);
      ReturnType runOnModule(Module& pModule) override;
    private:
      TargetBackend *m_pBackend;
      CortexMBackendMeta *m_pMeta;
  };

  CortexmReadShiftPass *CreateCortexmReadShiftPass(TargetBackend *pBackend , CortexMBackendMeta *pMeta);//pass function order

}


#endif
