#include <onnc/Analysis/Counter.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Module.h>

#include "CLangGetOperatorListPass.h"

namespace onnc {

CLangGetOperatorListPass::ReturnType CLangGetOperatorListPass::runOnModule(Module& module)
{
  // For each compute operator in module, add it into a list for
  // later usage when generating service library file
  for (ComputeOperator &cm : *module.getRootComputeGraph()) {
    if (dyn_cast<InputOperator>(&cm)) continue;
    if (dyn_cast<Initializer>(&cm)) continue;
    if (dyn_cast<OutputOperator>(&cm)) continue;
    onnc::StringRef name = cm.name(); 
    meta.operator_list.insert({name, NULL});
  }

  outs() << "[Clang] Operators:" << std::endl;
  for (const auto &op : meta.operator_list) {
    outs() << "[Clang]  - " << op.first << std::endl;
  }
  return kModuleNoChanged;
}

} // namespace onnc
