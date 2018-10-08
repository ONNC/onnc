//===- VanillaBackend.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "VanillaBackend.h"
#include "TargetInfo/VanillaTargetInfo.h"
#include "TargetInfo/VanillaTargetMemInfo.h"
#include "CodeEmitVisitor.h"

#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Analysis/NodeIRScheduler.h>
#include <onnc/CodeGen/BuildMemOperand.h>
#include <onnc/CodeGen/LinearScanMemAlloc.h>
#include <onnc/CodeGen/LiveIntervals.h>
#include <onnc/CodeGen/LiveValueMatrix.h>
#include <onnc/CodeGen/SetMemOperand.h>
#include <onnc/CodeGen/SlotIndexes.h>
#include <onnc/IR/CodeEmit.h>
#include <onnc/Target/TargetRegistry.h>
#include <onnc/Transforms/BookONNXGraphs.h>
#include <onnc/Transforms/BuildInitializers.h>
#include <onnc/Transforms/BuildInputOperators.h>
#include <onnc/Transforms/BuildOutputOperators.h>
#include <onnc/Transforms/DeadNodeElimination.h>
#include <onnc/Transforms/RemoveTrainingNodes.h>
#include <onnc/Transforms/TensorSel.h>
#include <onnc/Transforms/TensorSel/Standards/AddLower.h>
#include <onnc/Transforms/TensorSel/Standards/AveragePoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/BatchNormalizationLower.h>
#include <onnc/Transforms/TensorSel/Standards/ConcatLower.h>
#include <onnc/Transforms/TensorSel/Standards/ConvLower.h>
#include <onnc/Transforms/TensorSel/Standards/FlattenLower.h>
#include <onnc/Transforms/TensorSel/Standards/GemmLower.h>
#include <onnc/Transforms/TensorSel/Standards/GlobalAveragePoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/LeakyReluLower.h>
#include <onnc/Transforms/TensorSel/Standards/LRNLower.h>
#include <onnc/Transforms/TensorSel/Standards/MaxPoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/MulLower.h>
#include <onnc/Transforms/TensorSel/Standards/PReluLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReluLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReshapeLower.h>
#include <onnc/Transforms/TensorSel/Standards/SoftmaxLower.h>
#include <onnc/Transforms/TensorSel/Standards/SplitLower.h>
#include <onnc/Transforms/TensorSel/Standards/SumLower.h>
#include <onnc/Transforms/TensorSel/Standards/TransposeLower.h>
#include <onnc/Transforms/TensorSel/Standards/UpsampleLower.h>


using namespace onnc;

//===----------------------------------------------------------------------===//
// VanillaBackend
//===----------------------------------------------------------------------===//
VanillaBackend::VanillaBackend(const TargetOptions& pOptions)
  : TargetBackend(pOptions) { 
  m_pMemInfo = new VanillaTargetMemInfo();
}

VanillaBackend::~VanillaBackend()
{
  delete m_pMemInfo;
}

void VanillaBackend::addTensorSel(PassManager& pPM)
{
  errs() << "Vanilla is invoked\n";

  // target independent pass
  pPM.add(CreateRemoveTrainingNodesPass());
  pPM.add(CreateUpdateGraphOutputSizePass());
  pPM.add(CreateDeadNodeEliminationPass());
  pPM.add(CreateBookONNXGraphs());
  pPM.add(CreateBuildInitializers());
  pPM.add(CreateBuildInputOperators());
  pPM.add(CreateTensorSel(this));
  pPM.add(CreateBuildOutputOperators());
}

void VanillaBackend::addTensorSched(PassManager& pPM)
{
  // After method AddTensorSel, operators have been scheduled in an
  // topological order, which indeed respects the data dependency but
  // is not optimized for any objective.
}

void VanillaBackend::addMemAlloc(PassManager& pPM)
{
  pPM.add(CreateBuildSlotIndexesPass());
  pPM.add(CreateLiveIntervalsPass());
  pPM.add(CreateLiveValueMatrixPass());
  pPM.add(CreateLinearScanMemAllocPass(this));
  pPM.add(CreateBuildMemOperandPass());
  pPM.add(CreateSetMemOperandPass());
}

void VanillaBackend::addCodeEmit(PassManager& pPM, const Path& pOutput)
{
  static vanilla::CodeEmitVisitor ceVisitor;
  pPM.add(CreateCodeEmitPass(ceVisitor));
}

void VanillaBackend::RegisterLowers(LowerRegistry& pRegistry) const
{
  pRegistry.emplace<AddLower>();
  pRegistry.emplace<AveragePoolLower>();
  pRegistry.emplace<BatchNormalizationLower>();
  pRegistry.emplace<ConcatLower>();
  pRegistry.emplace<ConvLower>();
  pRegistry.emplace<FlattenLower>();
  pRegistry.emplace<GemmLower>();
  pRegistry.emplace<GlobalAveragePoolLower>();
  pRegistry.emplace<LRNLower>();
  pRegistry.emplace<LeakyReluLower>();
  pRegistry.emplace<MaxPoolLower>();
  pRegistry.emplace<MulLower>();
  pRegistry.emplace<PReluLower>();
  pRegistry.emplace<ReluLower>();
  pRegistry.emplace<ReshapeLower>();
  pRegistry.emplace<SoftmaxLower>();
  pRegistry.emplace<SplitLower>();
  pRegistry.emplace<SumLower>();
  pRegistry.emplace<TransposeLower>();
  pRegistry.emplace<UpsampleLower>();
}


//===----------------------------------------------------------------------===//
// Non member functions
//===----------------------------------------------------------------------===//
TargetBackend* CreateVanillaBackend(const TargetOptions& pOptions)
{
  return new VanillaBackend(pOptions);
}

extern "C" void InitializeVanillaONNCBackend()
{
  onnc::TargetRegistry::RegisterTargetBackend(getTheVanillaTarget(),
      CreateVanillaBackend);
}

