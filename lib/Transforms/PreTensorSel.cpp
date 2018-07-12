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
Pass::ReturnType runOnModule(::onnc::Module &pModule)
{
  IRBuilder builder(pModule);

  // Create top-level graph
  if (pModule.getGraphIR()->has_name())
    builder.CreateComputeGraph(pModule.getGraphIR()->name());
  else
    builder.CreateComputeGraph("top-level");

  // Create initializer. In ONNC, initializer is a kind of ComputeOperator.
  // XXX: ONNX doesn't define new types for these data structures
  std::vector<::onnx::Tensor>::const_iterator tensor, tEnd =
      pModule.getGraphIR()->initializers().end();

  std::vector<std::string>::const_iterator tname =
      pModule.getGraphIR()->initializer_names().begin();

  tensor = pModule.getGraphIR()->initializers().begin();
  while (tensor != tEnd) {
    builder.AddComputeOp<onnc::Initializer>(*tname);
    // TODO: setTensor
    ++tensor;
    ++tname;
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
