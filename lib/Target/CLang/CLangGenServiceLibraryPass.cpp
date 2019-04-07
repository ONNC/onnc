#include "CLangGenServiceLibraryPass.h"

#include <onnc/IR/Module.h>

namespace onnc {

CLangGenServiceLibraryPass::ReturnType CLangGenServiceLibraryPass::runOnModule(Module& module)
{
  return kModuleNoChanged;
}

} // namespace onnc
