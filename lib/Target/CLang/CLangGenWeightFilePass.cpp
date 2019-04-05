#include <onnc/IR/Module.h>

#include "CLangGenWeightFilePass.h"

namespace onnc {

CLangGenWeightFilePass::ReturnType CLangGenWeightFilePass::runOnModule(Module& module)
{
  return kModuleNoChanged;
}

} // namespace onnc
