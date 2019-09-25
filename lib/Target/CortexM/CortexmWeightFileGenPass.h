#ifndef CORTEXM_HERDER_GEN_PASS_H
#define CORTEXM_HERDER_GEN_PASS_H

#include "CortexmMeta.h"
#include <onnc/Core/CustomPass.h>

namespace onnc {
class TargetBackend;

class CortexmWeightFileGenPass : public CustomPass<CortexmWeightFileGenPass> {
public:
  explicit CortexmWeightFileGenPass(TargetBackend* pBackend,
                                    CortexmBackendMeta* pMeta);
  ReturnType runOnModule(Module& pModule) override;

private:
  void generateWeightFile();

private:
  TargetBackend* m_pBackend;
  CortexmBackendMeta* m_pMeta;
};
}

#endif
