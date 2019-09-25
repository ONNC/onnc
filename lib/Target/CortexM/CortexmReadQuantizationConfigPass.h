#ifndef CORTEXM_READ_QUANTIZATION_CONFIG_PASS_H
#define CORTEXM_READ_QUANTIZATION_CONFIG_PASS_H

#include "CortexmMeta.h"
#include <onnc/Core/CustomPass.h>
#include <onnc/Support/Path.h>

namespace onnc {
class TargetBackend;

class CortexmReadQuantizationConfigPass : public CustomPass<CortexmReadQuantizationConfigPass> {
public:
  struct shift_list* save_shift = (shift_list*)malloc(sizeof(shift_list));

  int first = 0;

  explicit CortexmReadQuantizationConfigPass(TargetBackend* pBackend,
                                CortexmBackendMeta* pMeta, const Path file);
  ReturnType runOnModule(Module& pModule) override;

private:
  TargetBackend* m_pBackend;
  CortexmBackendMeta* m_pMeta;
  const Path m_file;
};
}

#endif
