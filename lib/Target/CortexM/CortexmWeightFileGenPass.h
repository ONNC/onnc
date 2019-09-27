#ifndef CORTEXM_HERDER_GEN_PASS_H
#define CORTEXM_HERDER_GEN_PASS_H

#include "CortexmMeta.h"

#include <onnc/Core/CustomPass.h>

namespace onnc {
class TargetBackend;

class CortexmWeightFileGenPass : public CustomPass<CortexmWeightFileGenPass>
{
public:
  explicit CortexmWeightFileGenPass(const CortexmBackendMeta& pMeta);
  ReturnType runOnModule(Module& pModule) override;

private:
  void generateWeightFile();

private:
  const CortexmBackendMeta& m_pMeta;
};
} // namespace onnc

#endif
