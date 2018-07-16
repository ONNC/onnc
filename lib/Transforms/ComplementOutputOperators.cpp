//===- ComplementOutputOperators.cpp --------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/ComplementOutputOperators.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/IRBuilder.h>
#include <onnx/common/ir.h>

using namespace onnc;

char ComplementOutputOperators::ID = 0;

//===----------------------------------------------------------------------===//
// ComplementOutputOperators
//===----------------------------------------------------------------------===//
Pass::ReturnType
ComplementOutputOperators::runOnGraphs(::onnx::Graph& pTG, ComputeGraph& pCG)
{
  for (::onnx::Value* v : pTG.outputs())
    pCG.addOperator<onnc::OutputOperator>(v->uniqueName());

  return Pass::kModuleChanged;
}

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
INITIALIZE_PASS(ComplementOutputOperators, "ComplementOutputOperators");

ModulePass *onnc::CreateComplementOutputOperators()
{
  return new ComplementOutputOperators();
}
