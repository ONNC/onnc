//===- CortexMBackend.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <memory>

#include "CortexMBackend.h"
#include "TargetInfo/CortexMTargetInfo.h"
#include "TargetInfo/CortexMTargetMemInfo.h"
#include "CodeEmitVisitor.h"
#include "CortexmInputPerProcessing.h"
#include "CortexmWeightFileGenPass.h"
#include "CortexmMainFileHeaderGenPass.h"
#include "CortexmReadShiftPass.h"

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
#include <onnc/Transforms/TensorSel/Standards/MatMulLower.h>

#include <memory>

using namespace onnc;

//===----------------------------------------------------------------------===//
// CortexMBackend
//===----------------------------------------------------------------------===//
CortexMBackend::CortexMBackend(const TargetOptions& pOptions)
  : TargetBackend(pOptions),
    m_pMeta{},
    m_CodeEmitVisitor{m_pMeta} { 
  m_pMemInfo = std::make_unique<CortexMTargetMemInfo>();
}

void CortexMBackend::addTensorSel(PassManager& pPM)
{
  errs() << "CortexM is invoked\n";

  // Do ONNX graph IR optimization here.

  // Translate from ONNX graph IR into ONNC IR
  addStandardTensorSel(pPM, *this);
  
  // Now ONNC IR is ready.
  // If you need to extend ONNC IR, here is the place to add your pass that
  // adds your ONNC IR operators.
}

void CortexMBackend::addTensorSched(PassManager& pPM)
{
  // After method AddTensorSel, operators have been scheduled in an
  // topological order, which totally respects the data dependency.
  // However, that might not be an optimized order for certain objective.
  // Add a scheduling optimization pass here.
}

void CortexMBackend::addMemAlloc(PassManager& pPM)
{
  // Input: Module
  // Output: LiveIntervals
  addStandardCreateLiveIntervals(pPM);

  // Input: LiveIntervals
  // Output: MemAllocs
  addStandardMemoryAllocation(pPM, *this);

  // Input: MemAllocs
  // Output: Virtual memory address for each memory operands.
  addStandardSetMemOperands(pPM);
}

void CortexMBackend::addCodeEmit(PassManager& pPM, const Path& pOutput)
{
  // this is the old-style calling method. Do not use it.
  //pPM.add(CreateCodeEmitPass(m_CodeEmitVisitor));

  // use this new style. Refer to lib/Target/NvDla/NvDlaBackend.cpp
  pPM.add<CodeEmit>(m_CodeEmitVisitor);
  pPM.add<CortexmReadShiftPass>(&m_pMeta);
  pPM.add<CortexmHeaderFileGenPass>(&m_pMeta);
  pPM.add<CortexmInputPerProcessing>(&m_pMeta);
  pPM.add<CortexmMainFileHeaderGenPass>(&m_pMeta);
  pPM.add<CortexmFileGenPass>(&m_pMeta);
}

void CortexMBackend::RegisterLowers(LowerRegistry& pRegistry) const
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
TargetBackend* CreateCortexMBackend(const TargetOptions& pOptions)
{
  return new CortexMBackend(pOptions);
}

extern "C" void InitializeCortexMONNCBackend()
{
  onnc::TargetRegistry::RegisterTargetBackend(getTheCortexMTarget(),
      CreateCortexMBackend);
}


