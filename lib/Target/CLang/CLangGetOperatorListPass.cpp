#include "CLangGetOperatorListPass.h"

#include <onnc/Analysis/Counter.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Module.h>

namespace onnc {

CLangGetOperatorListPass::ReturnType CLangGetOperatorListPass::runOnModule(Module& module)
{
  for (ComputeOperator& cm : *module.getRootComputeGraph()) {
    if (isa<InputOperator>(&cm) || isa<Initializer>(&cm) || isa<OutputOperator>(&cm)) {
      continue;
    }

    meta.usedOperatorNames.insert(cm.name().str());
  }

  return kModuleNoChanged;
}

} // namespace onnc
