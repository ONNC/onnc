//===- ComplementInitializers.cpp -----------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/ComplementInitializers.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/IRBuilder.h>
#include <onnx/common/ir.h>

using namespace onnc;

char ComplementInitializers::ID = 0;

//===----------------------------------------------------------------------===//
// ComplementInitializers
//===----------------------------------------------------------------------===//
Pass::ReturnType
ComplementInitializers::runOnGraphs(::onnx::Graph& pTG, ComputeGraph& pCG)
{
  // Create initializer. In ONNC, initializer is a kind of ComputeOperator.
  // XXX: ONNX doesn't define new types for these data structures
  std::vector<::onnx::Tensor>::const_iterator tensor, tEnd =
      pTG.initializers().end();

  std::vector<std::string>::const_iterator tname =
      pTG.initializer_names().begin();

  tensor = pTG.initializers().begin();

  std::map<std::string, const ::onnx::Tensor*> initializers;
  while (tensor != tEnd) {
    initializers[*tname] = &*tensor;
    ++tname;
    ++tensor;
  }

  for (::onnx::Value* v : pTG.inputs()) {
    auto it = initializers.find(v->uniqueName());
    if (initializers.end() != it) {
      // The value appears in an initializer, we should create corresponding
      // initializer to handle with the value.
      Initializer* init = pCG.addOperator<onnc::Initializer>(v->uniqueName());
      Tensor* value = IRBuilder::CreateComputeTensor(pCG, *v, *it->second);
      init->setTensor(*value);
    }
  } // end of trip on all input values

  return Pass::kModuleChanged;
}

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
INITIALIZE_PASS(ComplementInitializers, "ComplementInitializers");

ModulePass *onnc::CreateComplementInitializers()
{
  return new ComplementInitializers();
}
