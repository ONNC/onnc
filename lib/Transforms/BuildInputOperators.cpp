//===- BuildInputOperators.cpp --------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/BuildInputOperators.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/IRBuilder.h>
#include <onnc/Config/ONNX.h>

using namespace onnc;

char BuildInputOperators::ID = 0;

//===----------------------------------------------------------------------===//
// BuildInputOperators
//===----------------------------------------------------------------------===//
Pass::ReturnType
BuildInputOperators::runOnGraphs(xGraph& pTG, ComputeGraph& pCG)
{
  const std::vector<std::string>& names = pTG.initializer_names();

  for (xValue* v : pTG.inputs()) {
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
INITIALIZE_PASS(BuildInputOperators, "BuildInputOperators");

ModulePass *onnc::CreateBuildInputOperators()
{
  return new BuildInputOperators();
}
