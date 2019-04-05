#include <onnc/IR/Module.h>

#include "CLangGenServiceLibraryPass.h"

namespace onnc {

CLangGenServiceLibraryPass::ReturnType CLangGenServiceLibraryPass::runOnModule(Module& module)
{
  return kModuleNoChanged;
}

} // namespace onnc
