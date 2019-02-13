//===- NvDlaBackend.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <memory>

#include "NvDlaBackend.h"
#include "TargetInfo/NvDlaTargetInfo.h"
#include "TargetInfo/NvDlaTargetMemInfo.h"
#include "NvDlaMemInfoPass.h"
#include "CodeEmitVisitor.h"
#include "NvDlaTaskSubmitPass.h"
#include "NvDlaFileGenPass.h"

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

#include <onnc/Transforms/TensorSel/Standards/UnsqueezeLower.h>


using namespace onnc;

//===----------------------------------------------------------------------===//
// NvDlaBackend
//===----------------------------------------------------------------------===//
NvDlaBackend::NvDlaBackend(const TargetOptions& pOptions)
  : TargetBackend{pOptions}
  , m_pMeta{}
  , m_CodeEmitVisitor{m_pMeta} {
  m_pMemInfo = std::make_unique<NvDlaTargetMemInfo>();
}

void NvDlaBackend::addTensorSel(PassManager& pPM)
{
  errs() << "NvDla is invoked\n";

  // Do ONNX graph IR optimization here.

  // Translate from ONNX graph IR into ONNC IR
  addStandardTensorSel(pPM, *this);

  // Now ONNC IR is ready.
  // If you need to extend ONNC IR, here is the place to add your pass that
  // adds your ONNC IR operators.
}

void NvDlaBackend::addTensorSched(PassManager& pPM)
{
  // After method AddTensorSel, operators have been scheduled in an
  // topological order, which totally respects the data dependency.
  // However, that might not be an optimized order for certain objective.
  // Add a scheduling optimization pass here.
}

void NvDlaBackend::addMemAlloc(PassManager& pPM)
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

void NvDlaBackend::addCodeEmit(PassManager& pPM, const Path& pOutput)
{
  pPM.add<NvDlaMemInfoPass>(&m_pMeta)
     .add<CodeEmit>(m_CodeEmitVisitor)
     .add<NvDlaTaskSubmitPass>(&m_pMeta)
     .add<NvDlaFileGenPass>(&m_pMeta)
     ;
}

void NvDlaBackend::RegisterLowers(LowerRegistry& pRegistry) const
{
  //CONV
  pRegistry.emplace<ConvLower>();
  pRegistry.emplace<GemmLower>();

  //SDP
  //pRegistry.emplace<PReluLower>();
  pRegistry.emplace<ReluLower>();
  //pRegistry.emplace<MulLower>();
  //pRegistry.emplace<AddLower>();
  //pRegistry.emplace<SumLower>();  // N Adds
  //pRegistry.emplace<BatchNormalizationLower>();

  //PDP
  pRegistry.emplace<MaxPoolLower>();
  pRegistry.emplace<AveragePoolLower>();

  //CDP
  pRegistry.emplace<LRNLower>();

  //RUBIK
  pRegistry.emplace<ReshapeLower>();    //special processing
  //pRegistry.emplace<SplitLower>();      //RUBIK
  //pRegistry.emplace<FlattenLower>();    //RUBIK

  //EMU
  pRegistry.emplace<SoftmaxLower>();

  //TODOs
  //pRegistry.emplace<LeakyReluLower>();  //SDP-LUT
  //pRegistry.emplace<GlobalAveragePoolLower>();  //PDP?
  //pRegistry.emplace<TransposeLower>();  //??
  //pRegistry.emplace<UpsampleLower>();   // BDMA + CONV??
  //pRegistry.emplace<UnsqueezeLower>();
  pRegistry.emplace<ConcatLower>();    // RUBIK?

}


//===----------------------------------------------------------------------===//
// Non member functions
//===----------------------------------------------------------------------===//
TargetBackend* CreateNvDlaBackend(const TargetOptions& pOptions)
{
  return new NvDlaBackend(pOptions);
}

extern "C" void InitializeNvDlaONNCBackend()
{
  onnc::TargetRegistry::RegisterTargetBackend(getTheNvDlaTarget(),
      CreateNvDlaBackend);
}
