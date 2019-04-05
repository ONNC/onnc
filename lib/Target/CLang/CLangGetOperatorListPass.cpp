#include <onnc/IR/Module.h>

#include "CLangGetOperatorListPass.h"

namespace onnc {

CLangGetOperatorListPass::ReturnType CLangGetOperatorListPass::runOnModule(Module& module)
{
  /// TODO: for each compute operators in module, add them into a list for use
  ///       later when generating service library file
  return kModuleNoChanged;
}

} // namespace onnc
