//===- TargetStandardPasses.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/CodeGen/BuildMemOperand.h>
#include <onnc/CodeGen/LinearScanMemAlloc.h>
#include <onnc/CodeGen/LiveIntervals.h>
#include <onnc/CodeGen/LiveValueMatrix.h>
#include <onnc/CodeGen/MemAllocData.h>
#include <onnc/CodeGen/SetMemOperand.h>
#include <onnc/CodeGen/SlotIndexes.h>
#include <onnc/Core/InitializePasses.h>
#include <onnc/Core/PassManager.h>
#include <onnc/Target/TargetStandardPasses.h>
#include <onnc/Transforms/BookONNXGraphs.h>
#include <onnc/Transforms/BuildInitializers.h>
#include <onnc/Transforms/BuildInputOperators.h>
#include <onnc/Transforms/BuildOutputOperators.h>
#include <onnc/Transforms/DeadNodeElimination.h>
#include <onnc/Transforms/RemoveTrainingNodes.h>
#include <onnc/Transforms/TensorSel.h>

#include <onnc/Option/CommandLine.h>

using namespace onnc;

cl::opt<std::string>
onnc::LinearScanAlgo("fLinearScanAlgo",
                     cl::kShort, cl::kOptional,
                     cl::kValueOptional, cl::kEqualSeparated,
                     cl::init("first-fit"),
                     cl::desc("Select linear scan algorithm: first-fit, best-fit. (default is first-fit)"));

//===----------------------------------------------------------------------===//
// TargetStandardPasses
//===----------------------------------------------------------------------===//
void onnc::addStandardTensorSel(PassManager& pPM, TargetBackend& pTB) {
  // ONNC is currently for Inferencing only. So we just remove the nodes that
  // only operate at training phase.
  pPM.add(CreateRemoveTrainingNodesPass());
  // Do the shape inference. The standard lower needs shape information.
  pPM.add(CreateUpdateGraphOutputSizePass());
  // Remove unused nodes. Standard Lower doesn't handle the "undefined" node.
  pPM.add(CreateDeadNodeEliminationPass());

  // Create ComputeGraph.
  pPM.add(CreateBookONNXGraphs());
  // Build the Initializer Operator (for the Weights).
  pPM.add(CreateBuildInitializers());
  // Build the Input Operator (for the Inputs).
  pPM.add(CreateBuildInputOperators());
  // Do the standard Tensor Selection, which uses LowerRegistry to get the
  // standard Lower, so you have to implement TargetBackend::RegisterLowers().
  pPM.add(CreateTensorSel(&pTB));
  // Build the Output Operator (for the Outputs).
  pPM.add(CreateBuildOutputOperators());
}

void onnc::addStandardCreateLiveIntervals(PassManager& pPM)
{
  PassRegistry& reg = *pPM.getPassRegistry();
  // Build slot id for each onnc ir
  InitializeBuildSlotIndexesPass(reg);
  // Data pass for saving live intervals built by LiveIntervals.
  InitializeLiveIntervalsDataPass(reg);
  // Build live interval for each value
  InitializeLiveIntervalsPass(reg);

  pPM.add(CreateLiveIntervalsPass());
}

void onnc::addStandardMemoryAllocation(PassManager& pPM, TargetBackend& pTB)
{
  PassRegistry& reg = *pPM.getPassRegistry();
  // Create live matrix for interference query
  InitializeLiveValueMatrixPass(reg);
  // Create memory allocation data pass for saving allocation result.
  InitializeMemAllocDataPass(reg);
  // Standard memory allocation for each value
  InitializeLinearScanMemAllocPass(reg);

  pPM.add(CreateLinearScanMemAllocPass(&pTB));
}

void onnc::addStandardSetMemOperands(PassManager& pPM)
{
  // Build onnc memory operands. Memory operand is an interface between memory
  // allocation and code emit (or jit/interpreter). Memory operand records
  // allocated address and size.
  pPM.add(CreateBuildMemOperandPass());
  // Fill allocated to memory operands.
  pPM.add(CreateSetMemOperandPass());
}
