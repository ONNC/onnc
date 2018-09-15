//===- SetMemOperand.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/CodeGen/LinearScanMemAlloc.h>
#include <onnc/CodeGen/SetMemOperand.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/Core/PassSupport.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// SetMemOperand
//===----------------------------------------------------------------------===//
Pass::ReturnType SetMemOperand::runOnModule(Module& pModule)
{
  // TODO: allow to use different memory allocation pass.
  LinearScanMemAlloc* memAllocPass = getAnalysis<LinearScanMemAlloc>();

  for (ComputeOperand* opnd : pModule.getComputeOperands()) {
    Value* v = opnd->getValue();
    if (!v)
      continue;

    LinearScanMemAlloc::AllocEntry alloc = memAllocPass->getAlloc(v);
    // FIXME: need some check, e.g isa<>
    ComputeMemOperand* memOpnd = (ComputeMemOperand*)opnd;
    memOpnd->setStart(alloc.startAddr);
    memOpnd->setLength(alloc.size);
  }

  return Pass::kModuleChanged;
}

void SetMemOperand::getAnalysisUsage(AnalysisUsage& pUsage) const
{
  pUsage.addRequiredID(LinearScanMemAlloc::ID);
}

//===----------------------------------------------------------------------===//
// SetMemOperand Factory method
//===----------------------------------------------------------------------===//
char SetMemOperand::ID = 0;

namespace onnc
{
  INITIALIZE_PASS(SetMemOperand, "SetMemOperand")
}

ModulePass* onnc::CreateSetMemOperandPass()
{
  return new SetMemOperand();
}