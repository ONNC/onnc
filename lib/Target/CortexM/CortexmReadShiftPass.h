#ifndef CORTEXM_READ_SHIFT_PASS
#define CORTEXM_READ_SHIFT_PASS

#include "CortexmMeta.h"
#include <onnc/Core/CustomPass.h>
#include <onnc/Support/Path.h>

namespace onnc {
class TargetBackend;

class CortexmReadShiftPass : public CustomPass<CortexmReadShiftPass> {
public:
  struct shift_list* save_shift = (shift_list*)malloc(sizeof(shift_list));

  int first = 0;

  explicit CortexmReadShiftPass(TargetBackend* pBackend,
                                CortexmBackendMeta* pMeta, const Path file);
  ReturnType runOnModule(Module& pModule) override;

private:
  TargetBackend* m_pBackend;
  CortexmBackendMeta* m_pMeta;
  const Path m_file;
};
}

#endif
