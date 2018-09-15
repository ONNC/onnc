//===- X86Backend.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "X86Backend.h"
#include "TargetInfo/X86TargetInfo.h"
#include "TargetInfo/X86TargetMemInfo.h"
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/CodeGen/BuildMemOperand.h>
#include <onnc/CodeGen/LinearScanMemAlloc.h>
#include <onnc/CodeGen/LiveIntervals.h>
#include <onnc/CodeGen/LiveValueMatrix.h>
#include <onnc/CodeGen/SetMemOperand.h>
#include <onnc/CodeGen/SlotIndexes.h>
#include <onnc/Target/TargetRegistry.h>
#include <onnc/Transforms/BookONNXGraphs.h>
#include <onnc/Transforms/BuildInitializers.h>
#include <onnc/Transforms/BuildInputOperators.h>
#include <onnc/Transforms/BuildOutputOperators.h>
#include <onnc/Transforms/DeadNodeElimination.h>
#include <onnc/Transforms/RemoveTrainingNodes.h>
#include <onnc/Transforms/TensorSel.h>
#include <onnc/Transforms/TensorSel/Standards/AbsLower.h>
#include <onnc/Transforms/TensorSel/Standards/AcosLower.h>
#include <onnc/Transforms/TensorSel/Standards/AddLower.h>
#include <onnc/Transforms/TensorSel/Standards/AffineLower.h>
#include <onnc/Transforms/TensorSel/Standards/AndLower.h>
#include <onnc/Transforms/TensorSel/Standards/ATenLower.h>
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
#include <onnc/Transforms/TensorSel/Standards/XorLower.h>
#include <onnc/Transforms/TensorSel/Standards/TransposeLower.h>
#include <onnc/Transforms/TensorSel/Standards/UpsampleLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// X86Backend
//===----------------------------------------------------------------------===//
X86Backend::X86Backend(const TargetOptions& pOptions)
  : NPUTargetBackend(pOptions) {
  m_pMemInfo = new X86TargetMemInfo();
}

X86Backend::~X86Backend()
{
  delete m_pMemInfo;
}

void X86Backend::addTensorSel(PassManager& pPM)
{
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

void X86Backend::addMemAlloc(PassManager& pPM)
{
  pPM.add(CreateBuildSlotIndexesPass());
  pPM.add(CreateLiveIntervalsPass());
  pPM.add(CreateLiveValueMatrixPass());
  pPM.add(CreateLinearScanMemAllocPass(this));
  pPM.add(CreateBuildMemOperandPass());
  pPM.add(CreateSetMemOperandPass());
}

void X86Backend::addCodeEmit(PassManager& pPM, const Path& pOutput)
{
  // TODO
}

void X86Backend::RegisterLowers(LowerRegistry& pRegistry) const
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
// X86_32Backend
//===----------------------------------------------------------------------===//
X86_32Backend::X86_32Backend(const TargetOptions& pOptions)
  : X86Backend(pOptions) {
}

X86_32Backend::~X86_32Backend()
{
}

void X86_32Backend::RegisterLowers(LowerRegistry& pRegistry) const
{
  X86Backend::RegisterLowers(pRegistry);
}

//===----------------------------------------------------------------------===//
// X86_64Backend
//===----------------------------------------------------------------------===//
X86_64Backend::X86_64Backend(const TargetOptions& pOptions)
  : X86Backend(pOptions) {
}

X86_64Backend::~X86_64Backend()
{
}

void X86_64Backend::RegisterLowers(LowerRegistry& pRegistry) const
{
  X86Backend::RegisterLowers(pRegistry);
}

//===----------------------------------------------------------------------===//
// Non member functions
//===----------------------------------------------------------------------===//
TargetBackend* CreateX86_32Backend(const TargetOptions& pOptions)
{
  return new X86_32Backend(pOptions);
}

TargetBackend* CreateX86_64Backend(const TargetOptions& pOptions)
{
  return new X86_64Backend(pOptions);
}

extern "C" void InitializeX86ONNCBackend()
{
  onnc::TargetRegistry::RegisterTargetBackend(getTheX86_32Target(),
      CreateX86_32Backend);
  onnc::TargetRegistry::RegisterTargetBackend(getTheX86_64Target(),
      CreateX86_64Backend);
}
