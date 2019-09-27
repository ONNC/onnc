#ifndef CORTEXM_MAIN_FILE_HEADER_GEN_PASS_H
#define CORTEXM_MAIN_FILE_HEADER_GEN_PASS_H

#include "CortexmBackendMeta.h"

#include <onnc/Core/CustomPass.h>

namespace onnc {
class TargetBackend;

class CortexmMainFileHeaderGenPass : public CustomPass<CortexmMainFileHeaderGenPass>
{
public:
  CortexmMainFileHeaderGenPass() = default;
  ReturnType runOnModule(Module& pModule) override;
};
} // namespace onnc

#endif
