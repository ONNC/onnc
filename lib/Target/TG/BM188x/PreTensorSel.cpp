//===- PreTensorSel.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "PreTensorSel.h"
#include "../TG.h"
#include <onnc/IR/IRBuilder.h>
#include <onnx/common/ir.h>
#include <onnc/IR/Compute/Initializer.h>

using namespace onnc;

char bm188x::PreTensorSel::ID = 0;

//===----------------------------------------------------------------------===//
// bm188x::PreTensorSel
//===----------------------------------------------------------------------===//
bm188x::PreTensorSel::PreTensorSel(BM1880Backend *pBackend)
  : ModulePass(ID), m_pTarget(pBackend) {
}

Pass::ReturnType bm188x::PreTensorSel::runOnModule(::onnc::Module &pModule)
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
    Initializer* initializer = builder.AddComputeOp<onnc::Initializer>(*tname);
    initializer->setTensor(tensor++);
    initializer->setName(tname++);
  }
  return Pass::kModuleNoChanged;
}

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
ModulePass* onnc::createPreTensorSel(BM1880Backend *pTarget)
{
  return new bm188x::PreTensorSel(pTarget);
}
