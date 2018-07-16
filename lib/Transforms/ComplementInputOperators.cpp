//===- ComplementInputOperators.cpp ---------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/ComplementInputOperators.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/IRBuilder.h>
#include <onnx/common/ir.h>

using namespace onnc;

char ComplementInputOperators::ID = 0;

//===----------------------------------------------------------------------===//
// ComplementInputOperators
//===----------------------------------------------------------------------===//
Pass::ReturnType
ComplementInputOperators::runOnGraphs(::onnx::Graph& pTG, ComputeGraph& pCG)
{
  const std::vector<std::string>& names = pTG.initializer_names();

  for (::onnx::Value* v : pTG.inputs()) {
    auto it = find(names.begin(), names.end(), v->uniqueName());
    if (names.end() == it) {
      // The value appears in an initializer, we should create corresponding
      // initializer to handle with the value.
      onnc::InputOperator* init = pCG.addOperator<onnc::InputOperator>(v->uniqueName());
      Tensor* value = IRBuilder::CreateComputeTensor(pCG, *v);
      init->setTensor(*value);
    }
  } // end of trip on all input values

  return Pass::kModuleChanged;
}

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
INITIALIZE_PASS(ComplementInputOperators, "ComplementInputOperators");

ModulePass *onnc::CreateComplementInputOperators()
{
  return new ComplementInputOperators();
}
