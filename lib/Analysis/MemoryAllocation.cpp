//===- MemoryAllocation.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/MemoryAllocation.h>
#include <onnc/Analysis/LivenessAnalysis.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Core/AnalysisUsage.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//


//===----------------------------------------------------------------------===//
// MemoryAllocation
//===----------------------------------------------------------------------===//
MemoryAllocation::MemoryAllocation()
  : ModulePass(ID) {
}

bool MemoryAllocation::runOnModule(Module& pModule)
{
  // Try to allocate virtual memory according to liveness range.
  for (onnx::Node *n : pModule.getGraph()->nodes()) {
    // getAnalysis<GraphLivenessAnalysis>(pModule)

    // get required memory size of each input.

    // get required memory size of each output.

    // allocate memory considering liveness.
  }
  
  return false;
}

void MemoryAllocation::getAnalysisUsage(AnalysisUsage& pUsage) const
{
  pUsage.addRequiredID(GraphLivenessAnalysis::ID);
  pUsage.addRequiredID(UpdateGraphOutputSize::ID);
}


//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char MemoryAllocation::ID = 0;

MemoryAllocation* onnc::CreateMemoryAllocationPass()
{
  return new MemoryAllocation();
}
