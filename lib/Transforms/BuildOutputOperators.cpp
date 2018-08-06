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
#include <onnc/Config/ONNX.h>

using namespace onnc;

char BuildOutputOperators::ID = 0;

//===----------------------------------------------------------------------===//
// BuildOutputOperators
//===----------------------------------------------------------------------===//
Pass::ReturnType
BuildOutputOperators::runOnGraphs(xGraph& pTG, ComputeGraph& pCG)
{
  for (xValue* v : pTG.outputs()) {
    OutputOperator* op = pCG.addOperator<onnc::OutputOperator>(v->uniqueName());
    // add v as an input.
    onnc::Tensor* tensor = pCG.getValue<onnc::Tensor>(v->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pCG, *v);
    op->addInput(*tensor);
  }

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
