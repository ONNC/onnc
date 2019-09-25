#ifndef ONNC_CORTEXM_INPUT_PRE_REOCESSING_FILE_GEN_PASS_H
#define ONNC_CORTEXM_INPUT_PRE_REOCESSING_FILE_GEN_PASS_H

#include "CortexmMeta.h"
#include <onnc/Core/CustomPass.h>

namespace onnc {
class TargetBackend;

class CortexmInputPreProcessingFileGenPass : public CustomPass<CortexmInputPreProcessingFileGenPass> {
public:
  explicit CortexmInputPreProcessingFileGenPass(TargetBackend* pBackend,
                                     CortexmBackendMeta* pMeta);
  ReturnType runOnModule(Module& pModule) override;

private:
  TargetBackend* m_pBackend;
  CortexmBackendMeta* m_pMeta;
};
}

#endif
