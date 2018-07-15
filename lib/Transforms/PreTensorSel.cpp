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

  std::map<std::string, const ::onnx::Tensor*> initializers;
  while (tensor != tEnd) {
    initializers[*tname] = &*tensor;
    ++tname;
    ++tensor;
  }

  for (::onnx::Value* v : graph->inputs()) {
    auto it = initializers.find(v->uniqueName());
    if (initializers.end() == it) {
      // If a value doesn't appear in initilizers, then we only need to keep
      // its shape.
      builder.CreateComputeTensor(*v);
    }
    else {
      // The value appears in an initializer, we should create corresponding
      // initializer to handle with the value.
      Initializer* init =
          builder.AddComputeOp<onnc::Initializer>(v->uniqueName());
      Tensor* value = builder.CreateComputeTensor(*v, *it->second);
      init->setTensor(*value);
    }
  } // end of trip on all input values

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
