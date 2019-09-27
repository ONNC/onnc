#ifndef ONNC_CORTEXM_INPUT_PRE_REOCESSING_FILE_GEN_PASS_H
#define ONNC_CORTEXM_INPUT_PRE_REOCESSING_FILE_GEN_PASS_H

#include "CortexmBackendMeta.h"

#include <onnc/Core/CustomPass.h>

namespace onnc {
class TargetBackend;

class CortexmInputPreProcessingFileGenPass : public CustomPass<CortexmInputPreProcessingFileGenPass>
{
public:
  CortexmInputPreProcessingFileGenPass() = default;
  ReturnType runOnModule(Module& pModule) override;
};
} // namespace onnc

#endif
