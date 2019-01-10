//===- SetMemOperand.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/CodeGen/MemAllocData.h>
#include <onnc/CodeGen/SetMemOperand.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/Core/PassSupport.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// SetMemOperand
//===----------------------------------------------------------------------===//
Pass::ReturnType SetMemOperand::runOnModule(Module& pModule)
{
  MemAllocData* memAllocData = getAnalysis<MemAllocData>();

  for (ComputeOperand* opnd : pModule.getComputeOperands()) {
    Value* v = opnd->getValue();
    if (!v || !memAllocData->hasAlloc(v))
      continue;

    MemAllocData::AllocEntry alloc = memAllocData->getAlloc(v);
    // FIXME: need some check, e.g isa<>
    ComputeMemOperand* memOpnd = (ComputeMemOperand*)opnd;
    memOpnd->setStart(alloc.startAddr);
    memOpnd->setLength(alloc.size);
  }

  return Pass::kModuleChanged;
}

void SetMemOperand::getAnalysisUsage(AnalysisUsage& pUsage) const
{
  pUsage.addRequired<MemAllocData>();
}

//===----------------------------------------------------------------------===//
// SetMemOperand Factory method
//===----------------------------------------------------------------------===//
namespace onnc
{
  INITIALIZE_PASS(SetMemOperand, "SetMemOperand")
}

ModulePass* onnc::CreateSetMemOperandPass()
{
  return new SetMemOperand();
}
