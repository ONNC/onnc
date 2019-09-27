//===- CortexMBackend.cpp
//-----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "CortexmBackend.h"

#include "CodeEmitVisitor.h"
#include "CortexmInputPreProcessingFileGenPass.h"
#include "CortexmMainFileGenPass.h"
#include "CortexmMainFileHeaderGenPass.h"
#include "CortexmReadQuantizationConfigPass.h"
#include "CortexmWeightFileGenPass.h"
#include "TargetInfo/CortexMTargetInfo.h"
#include "TargetInfo/CortexMTargetMemInfo.h"

#include <onnc/Analysis/NodeIRScheduler.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/CodeGen/BuildMemOperand.h>
#include <onnc/CodeGen/LinearScanMemAlloc.h>
#include <onnc/CodeGen/LiveIntervals.h>
#include <onnc/CodeGen/LiveValueMatrix.h>
#include <onnc/CodeGen/SetMemOperand.h>
#include <onnc/CodeGen/SlotIndexes.h>
#include <onnc/IR/CodeEmit.h>
#include <onnc/Target/TargetRegistry.h>
#include <onnc/Target/TargetStandardPasses.h>
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
#include <onnc/Transforms/TensorSel/Standards/LRNLower.h>
#include <onnc/Transforms/TensorSel/Standards/LeakyReluLower.h>
#include <onnc/Transforms/TensorSel/Standards/MatMulLower.h>
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

#include <memory>

using namespace onnc;

//===----------------------------------------------------------------------===//
// CortexMBackend
//===----------------------------------------------------------------------===//
CortexmBackend::CortexmBackend(const TargetOptions& pOptions)
  : TargetBackend(pOptions)
  , m_pMeta{}
  , m_CodeEmitVisitor{m_pMeta}
{
  m_pMemInfo = std::make_unique<CortexMTargetMemInfo>();
}

void CortexmBackend::addTensorSel(PassManager& passManager)
{
  // Do ONNX graph IR optimization here.

  // Translate from ONNX graph IR into ONNC IR
  addStandardTensorSel(passManager, *this);

  // Now ONNC IR is ready.
  // If you need to extend ONNC IR, here is the place to add your pass that
  // adds your ONNC IR operators.
}

void CortexmBackend::addTensorSched(PassManager& passManager)
{
  // After method AddTensorSel, operators have been scheduled in an
  // topological order, which totally respects the data dependency.
  // However, that might not be an optimized order for certain objective.
  // Add a scheduling optimization pass here.
}

void CortexmBackend::addMemAlloc(PassManager& passManager)
{
  // Input: Module
  // Output: LiveIntervals
  addStandardCreateLiveIntervals(passManager);

  // Input: LiveIntervals
  // Output: MemAllocs
  addStandardMemoryAllocation(passManager, *this);

  // Input: MemAllocs
  // Output: Virtual memory address for each memory operands.
  addStandardSetMemOperands(passManager);
}

void CortexmBackend::addCodeEmit(PassManager& passManager, const Path& pOutput)
{
  // this is the old-style calling method. Do not use it.
  // passManager.add(CreateCodeEmitPass(m_CodeEmitVisitor));

  // use this new style. Refer to lib/Target/NvDla/NvDlaBackend.cpp
  passManager.add<CodeEmit>(m_CodeEmitVisitor);
  passManager.add<CortexmReadQuantizationConfigPass>(m_pMeta, options().getQuantizationConfigFile());
  passManager.add<CortexmWeightFileGenPass>(m_pMeta);
  passManager.add<CortexmInputPreProcessingFileGenPass>();
  passManager.add<CortexmMainFileHeaderGenPass>();
  passManager.add<CortexmMainFileGenPass>(m_pMeta);
}

void CortexmBackend::RegisterLowers(LowerRegistry& pRegistry) const
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
  pRegistry.emplace<MatMulLower>();
}

//===----------------------------------------------------------------------===//
// Non member functions
//===----------------------------------------------------------------------===//
TargetBackend* CreateCortexmBackend(const TargetOptions& pOptions) { return new CortexmBackend(pOptions); }

extern "C" void InitializeCortexMONNCBackend()
{
  onnc::TargetRegistry::RegisterTargetBackend(getTheCortexMTarget(), CreateCortexmBackend);
}
