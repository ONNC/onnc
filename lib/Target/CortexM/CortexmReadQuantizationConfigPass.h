#ifndef CORTEXM_READ_QUANTIZATION_CONFIG_PASS_H
#define CORTEXM_READ_QUANTIZATION_CONFIG_PASS_H

#include "CortexmMeta.h"

#include <onnc/Core/CustomPass.h>
#include <onnc/Support/Path.h>

namespace onnc {
class TargetBackend;

class CortexmReadQuantizationConfigPass : public CustomPass<CortexmReadQuantizationConfigPass>
{
public:
  explicit CortexmReadQuantizationConfigPass(CortexmBackendMeta& pMeta, const Path file);
  ReturnType runOnModule(Module& pModule) override;

private:
  CortexmBackendMeta& m_pMeta;
  const Path          m_file;
};
} // namespace onnc

#endif
