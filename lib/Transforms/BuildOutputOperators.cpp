//===- BuildOutputOperators.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/BuildOutputOperators.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/IRBuilder.h>
#include <onnx/common/ir.h>

using namespace onnc;

char BuildOutputOperators::ID = 0;

//===----------------------------------------------------------------------===//
// BuildOutputOperators
//===----------------------------------------------------------------------===//
Pass::ReturnType
BuildOutputOperators::runOnGraphs(::onnx::Graph& pTG, ComputeGraph& pCG)
{
  for (::onnx::Value* v : pTG.outputs())
    pCG.addOperator<onnc::OutputOperator>(v->uniqueName());

  return Pass::kModuleChanged;
}

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
INITIALIZE_PASS(BuildOutputOperators, "BuildOutputOperators");

ModulePass *onnc::CreateBuildOutputOperators()
{
  return new BuildOutputOperators();
}
