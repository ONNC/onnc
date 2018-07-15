//===- PreTensorSel.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/PreTensorSel.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/IRBuilder.h>
#include <onnx/common/ir.h>

using namespace onnc;

char PreTensorSel::ID = 0;

//===----------------------------------------------------------------------===//
// PreTensorSel
//===----------------------------------------------------------------------===//
Pass::ReturnType PreTensorSel::runOnModule(::onnc::Module &pModule)
{
  IRBuilder builder(pModule);
  ::onnx::Graph* graph = pModule.getGraphIR().get();

  // Create top-level graph
  if (graph->has_name())
    builder.CreateComputeGraph(graph->name());
  else
    builder.CreateComputeGraph("top-level");

  // Create initializer. In ONNC, initializer is a kind of ComputeOperator.
  // XXX: ONNX doesn't define new types for these data structures
  std::vector<::onnx::Tensor>::const_iterator tensor, tEnd =
      graph->initializers().end();

  std::vector<std::string>::const_iterator tname =
      graph->initializer_names().begin();

  tensor = graph->initializers().begin();
  std::map<std::string, const ::onnx::Tensor*> initializerInputs;
  while (tensor != tEnd) {
    initializerInputs[*tname++] = &*tensor++;
  }

  for (::onnx::Value* v : graph->inputs()) {
    auto it = initializerInputs.find(v->uniqueName());
    const ::onnx::Tensor* onnxTensor = it != initializerInputs.end() ?
      it->second : nullptr;

    Tensor* onncTensor = builder.CreateComputeTensor(*v, *onnxTensor);

    // Create initializer tensor
    if (onnxTensor) {
      Initializer* initializer =
        builder.AddComputeOp<onnc::Initializer>(v->uniqueName());
      initializer->setTensor(*onncTensor);
    }
  }

  return Pass::kModuleChanged;
}

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
INITIALIZE_PASS(PreTensorSel, "PreTensorSel");

ModulePass *onnc::CreatePreTensorSel()
{
  return new PreTensorSel();
}
