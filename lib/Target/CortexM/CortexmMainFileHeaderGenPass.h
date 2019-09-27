#ifndef CORTEXM_MAIN_FILE_HEADER_GEN_PASS_H
#define CORTEXM_MAIN_FILE_HEADER_GEN_PASS_H

#include "CortexmMeta.h"

#include <onnc/Core/CustomPass.h>

namespace onnc {
class TargetBackend;

class CortexmMainFileHeaderGenPass : public CustomPass<CortexmMainFileHeaderGenPass>
{
public:
  explicit CortexmMainFileHeaderGenPass() = default;
  ReturnType runOnModule(Module& pModule) override;
};
} // namespace onnc

#endif
