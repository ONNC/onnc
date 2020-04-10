//===- ExtractConstToInitializer.cpp ----------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Config/ONNX.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Transforms/ExtractConstToInitializer.h>

using namespace onnc;

Pass::ReturnType ExtractConstToInitializer::runOnModule(::onnc::Module &pModule)
{
  xGraph *graph = pModule.getRootTensorGraph();
  Pass::ReturnType isChanged = Pass::kModuleNoChanged;

  for (auto it = graph->begin(); it != graph->end(); ++it) {
    auto* node = *it;
    if (node->kind() == ONNX_NAMESPACE::kConstant) {
      isChanged |= Pass::kModuleChanged;

      const auto name = node->output()->uniqueName();
      
      // Get Constant attribute value
      xTensor t = node->t(ONNX_NAMESPACE::kvalue);
      /*
          Constant           Initializer
          (value)              (value)
             |          ->        |
        [output_tensor]     [output_tensor]
      */
      xValue* new_init = graph->addInitializerAndInput(t, name);
      node->output()->replaceAllUsesWith(new_init);
      it.destroyCurrent();
    }
  }
  return isChanged;
}

ModulePass *onnc::CreateExtractConstToInitializerPass()
{
  return new ExtractConstToInitializer();
}
