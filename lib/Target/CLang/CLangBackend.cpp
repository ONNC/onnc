//===- CLangBackend.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <memory>

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
#include <onnc/Transforms/TensorSel/Standards/AbsLower.h>
#include <onnc/Transforms/TensorSel/Standards/AcosLower.h>
#include <onnc/Transforms/TensorSel/Standards/AddLower.h>
#include <onnc/Transforms/TensorSel/Standards/AffineLower.h>
#include <onnc/Transforms/TensorSel/Standards/AndLower.h>
#include <onnc/Transforms/TensorSel/Standards/ArgMaxLower.h>
#include <onnc/Transforms/TensorSel/Standards/ArgMinLower.h>
#include <onnc/Transforms/TensorSel/Standards/AsinLower.h>
#include <onnc/Transforms/TensorSel/Standards/AtanLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/ATenLower.h>
#include <onnc/Transforms/TensorSel/Standards/AveragePoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/BatchNormalizationLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/CastLower.h>
#include <onnc/Transforms/TensorSel/Standards/CeilLower.h>
#include <onnc/Transforms/TensorSel/Standards/ClipLower.h>
#include <onnc/Transforms/TensorSel/Standards/ConcatLower.h>
// FIXME: #include <onnc/Transforms/TensorSel/Standards/ConstantLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/ConstantFillLower.h>
#include <onnc/Transforms/TensorSel/Standards/ConvLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/ConvTransposeLower.h>
#include <onnc/Transforms/TensorSel/Standards/CosLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/CropLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/DepthToSpaceLower.h>
#include <onnc/Transforms/TensorSel/Standards/DivLower.h>
#include <onnc/Transforms/TensorSel/Standards/DropoutLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/EluLower.h>
#include <onnc/Transforms/TensorSel/Standards/EqualLower.h>
#include <onnc/Transforms/TensorSel/Standards/ExpLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/ExpandLower.h>
#include <onnc/Transforms/TensorSel/Standards/FlattenLower.h>
#include <onnc/Transforms/TensorSel/Standards/FloorLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/GatherLower.h>
#include <onnc/Transforms/TensorSel/Standards/GemmLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/GivenTensorFillLower.h>
#include <onnc/Transforms/TensorSel/Standards/GlobalAveragePoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/GlobalLpPoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/GlobalMaxPoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/GreaterLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/GruLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/GruUnitLower.h>
#include <onnc/Transforms/TensorSel/Standards/HardmaxLower.h>
#include <onnc/Transforms/TensorSel/Standards/HardSigmoidLower.h>
#include <onnc/Transforms/TensorSel/Standards/IdentityLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/IfLower.h>
#include <onnc/Transforms/TensorSel/Standards/ImageScalerLower.h>
#include <onnc/Transforms/TensorSel/Standards/InstanceNormalizationLower.h>
#include <onnc/Transforms/TensorSel/Standards/LeakyReluLower.h>
#include <onnc/Transforms/TensorSel/Standards/LessLower.h>
#include <onnc/Transforms/TensorSel/Standards/LogLower.h>
#include <onnc/Transforms/TensorSel/Standards/LogSoftmaxLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/LoopLower.h>
#include <onnc/Transforms/TensorSel/Standards/LpNormalizationLower.h>
#include <onnc/Transforms/TensorSel/Standards/LpPoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/LRNLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/LSTMLower.h>
#include <onnc/Transforms/TensorSel/Standards/MatMulLower.h>
#include <onnc/Transforms/TensorSel/Standards/MaxLower.h>
#include <onnc/Transforms/TensorSel/Standards/MaxPoolLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/MaxRoiPoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/MeanLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/MeanVarianceNormalizationLower.h>
#include <onnc/Transforms/TensorSel/Standards/MinLower.h>
#include <onnc/Transforms/TensorSel/Standards/MulLower.h>
#include <onnc/Transforms/TensorSel/Standards/MultinomialLower.h>
#include <onnc/Transforms/TensorSel/Standards/NegLower.h>
#include <onnc/Transforms/TensorSel/Standards/NotLower.h>
#include <onnc/Transforms/TensorSel/Standards/OrLower.h>
#include <onnc/Transforms/TensorSel/Standards/PadLower.h>
#include <onnc/Transforms/TensorSel/Standards/ParametricSoftplusLower.h>
#include <onnc/Transforms/TensorSel/Standards/PowLower.h>
#include <onnc/Transforms/TensorSel/Standards/PReluLower.h>
#include <onnc/Transforms/TensorSel/Standards/RandomNormalLower.h>
#include <onnc/Transforms/TensorSel/Standards/RandomNormalLikeLower.h>
#include <onnc/Transforms/TensorSel/Standards/RandomUniformLower.h>
#include <onnc/Transforms/TensorSel/Standards/RandomUniformLikeLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReciprocalLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReduceL1Lower.h>
#include <onnc/Transforms/TensorSel/Standards/ReduceL2Lower.h>
#include <onnc/Transforms/TensorSel/Standards/ReduceLogSumLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReduceLogSumExpLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReduceMaxLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReduceMeanLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReduceMinLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReduceProdLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReduceSumLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReduceSumSquareLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReluLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReshapeLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/RnnLower.h>
#include <onnc/Transforms/TensorSel/Standards/ScaleLower.h>
#include <onnc/Transforms/TensorSel/Standards/ScaledTanhLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/ScanLower.h>
#include <onnc/Transforms/TensorSel/Standards/SeluLower.h>
#include <onnc/Transforms/TensorSel/Standards/ShapeLower.h>
#include <onnc/Transforms/TensorSel/Standards/SigmoidLower.h>
#include <onnc/Transforms/TensorSel/Standards/SinLower.h>
#include <onnc/Transforms/TensorSel/Standards/SizeLower.h>
#include <onnc/Transforms/TensorSel/Standards/SliceLower.h>
#include <onnc/Transforms/TensorSel/Standards/SoftmaxLower.h>
// TODO: #include <onnc/Transforms/TensorSel/Standards/SoftplusLower.h>
#include <onnc/Transforms/TensorSel/Standards/SoftsignLower.h>
#include <onnc/Transforms/TensorSel/Standards/SpaceToDepthLower.h>
#include <onnc/Transforms/TensorSel/Standards/SplitLower.h>
#include <onnc/Transforms/TensorSel/Standards/SqrtLower.h>
#include <onnc/Transforms/TensorSel/Standards/SqueezeLower.h>
#include <onnc/Transforms/TensorSel/Standards/SubLower.h>
#include <onnc/Transforms/TensorSel/Standards/SumLower.h>
#include <onnc/Transforms/TensorSel/Standards/TanLower.h>
#include <onnc/Transforms/TensorSel/Standards/TanhLower.h>
#include <onnc/Transforms/TensorSel/Standards/ThresholdedReluLower.h>
#include <onnc/Transforms/TensorSel/Standards/TileLower.h>
#include <onnc/Transforms/TensorSel/Standards/TopKLower.h>
#include <onnc/Transforms/TensorSel/Standards/TransposeLower.h>
#include <onnc/Transforms/TensorSel/Standards/UnsqueezeLower.h>
#include <onnc/Transforms/TensorSel/Standards/UpsampleLower.h>
#include <onnc/Transforms/TensorSel/Standards/XorLower.h>

#include "CLangBackend.h"
#include "CLangGenWeightFilePass.h"
#include "CLangGenServiceLibraryPass.h"
#include "CLangGetOperatorListPass.h"
#include "CLangMemInfoPass.h"
#include "TargetInfo/CLangTargetInfo.h"
#include "TargetInfo/CLangTargetMemInfo.h"

using namespace onnc;

//===----------------------------------------------------------------------===//
// CLangBackend
//===----------------------------------------------------------------------===//
CLangBackend::CLangBackend(const TargetOptions& pOptions)
  : TargetBackend(pOptions)
{ 
  m_pMemInfo = std::make_unique<CLangTargetMemInfo>();
}

void CLangBackend::addTensorSel(PassManager& pPM)
{
  errs() << "CLang is invoked\n";

  // Do ONNX graph IR optimization here.

  // Translate from ONNX graph IR into ONNC IR
  addStandardTensorSel(pPM, *this);
  
  // Now ONNC IR is ready.
  // If you need to extend ONNC IR, here is the place to add your pass that
  // adds your ONNC IR operators.
}

void CLangBackend::addTensorSched(PassManager& pPM)
{
  // After method AddTensorSel, operators have been scheduled in an
  // topological order, which totally respects the data dependency.
  // However, that might not be an optimized order for certain objective.
  // Add a scheduling optimization pass here.
}

void CLangBackend::addMemAlloc(PassManager& pPM)
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

void CLangBackend::addCodeEmit(PassManager& pPM, const Path& pOutput)
{
  Path weightFile = pOutput.parent().append(
    pOutput.stem().native() + m_pMeta.weight_extension
  );
  pPM.add<CLangMemInfoPass>(m_pMeta)
     .add<CLangGetOperatorListPass>(m_pMeta)
     .add<CLangGenWeightFilePass>(m_pMeta, std::move(weightFile))
     .add<CLangGenServiceLibraryPass>(m_pMeta, pOutput)
     // .add<CodeEmit>(m_CodeEmitVisitor);
     ;
}

void CLangBackend::RegisterLowers(LowerRegistry& pRegistry) const
{
  pRegistry.emplace<AbsLower>();
  pRegistry.emplace<AcosLower>();
  pRegistry.emplace<AddLower>();
  pRegistry.emplace<AffineLower>();
  pRegistry.emplace<AndLower>();
  pRegistry.emplace<ArgMaxLower>();
  pRegistry.emplace<ArgMinLower>();
  pRegistry.emplace<AsinLower>();
  pRegistry.emplace<AtanLower>();
  // TODO: pRegistry.emplace<ATenLower>();
  pRegistry.emplace<AveragePoolLower>();
  pRegistry.emplace<BatchNormalizationLower>();
  // TODO: pRegistry.emplace<CastLower>();
  pRegistry.emplace<CeilLower>();
  pRegistry.emplace<ClipLower>();
  pRegistry.emplace<ConcatLower>();
  // FIXME: pRegistry.emplace<ConstantLower>();
  // TODO: pRegistry.emplace<ConstantFillLower>();
  pRegistry.emplace<ConvLower>();
  // TODO: pRegistry.emplace<ConvTransposeLower>();
  pRegistry.emplace<CosLower>();
  // TODO: pRegistry.emplace<CropLower>();
  // TODO: pRegistry.emplace<DepthToSpaceLower>();
  pRegistry.emplace<DivLower>();
  pRegistry.emplace<DropoutLower>();
  // TODO: pRegistry.emplace<EluLower>();
  pRegistry.emplace<EqualLower>();
  pRegistry.emplace<ExpLower>();
  // TODO: pRegistry.emplace<ExpandLower>();
  pRegistry.emplace<FlattenLower>();
  pRegistry.emplace<FloorLower>();
  // TODO: pRegistry.emplace<GatherLower>();
  pRegistry.emplace<GemmLower>();
  // TODO: pRegistry.emplace<GivenTensorFillLower>();
  pRegistry.emplace<GlobalAveragePoolLower>();
  pRegistry.emplace<GlobalLpPoolLower>();
  pRegistry.emplace<GlobalMaxPoolLower>();
  pRegistry.emplace<GreaterLower>();
  // TODO: pRegistry.emplace<GruLower>();
  // TODO: pRegistry.emplace<GruUnitLower>();
  pRegistry.emplace<HardmaxLower>();
  pRegistry.emplace<HardSigmoidLower>();
  pRegistry.emplace<IdentityLower>();
  // TODO: pRegistry.emplace<IfLower>();
  pRegistry.emplace<ImageScalerLower>();
  pRegistry.emplace<InstanceNormalizationLower>();
  pRegistry.emplace<LeakyReluLower>();
  pRegistry.emplace<LessLower>();
  pRegistry.emplace<LogLower>();
  pRegistry.emplace<LogSoftmaxLower>();
  // TODO: pRegistry.emplace<LoopLower>();
  pRegistry.emplace<LpNormalizationLower>();
  pRegistry.emplace<LpPoolLower>();
  pRegistry.emplace<LRNLower>();
  // TODO: pRegistry.emplace<LSTMLower>();
  pRegistry.emplace<MatMulLower>();
  pRegistry.emplace<MaxLower>();
  pRegistry.emplace<MaxPoolLower>();
  // TODO: pRegistry.emplace<MaxRoiPoolLower>();
  pRegistry.emplace<MeanLower>();
  // TODO: pRegistry.emplace<MeanVarianceNormalizationLower>();
  pRegistry.emplace<MinLower>();
  pRegistry.emplace<MulLower>();
  pRegistry.emplace<MultinomialLower>();
  pRegistry.emplace<NegLower>();
  pRegistry.emplace<NotLower>();
  pRegistry.emplace<OrLower>();
  pRegistry.emplace<PadLower>();
  pRegistry.emplace<ParametricSoftplusLower>();
  pRegistry.emplace<PowLower>();
  pRegistry.emplace<PReluLower>();
  pRegistry.emplace<RandomNormalLower>();
  pRegistry.emplace<RandomNormalLikeLower>();
  pRegistry.emplace<RandomUniformLower>();
  pRegistry.emplace<RandomUniformLikeLower>();
  pRegistry.emplace<ReciprocalLower>();
  pRegistry.emplace<ReduceL1Lower>();
  pRegistry.emplace<ReduceL2Lower>();
  pRegistry.emplace<ReduceLogSumLower>();
  pRegistry.emplace<ReduceLogSumExpLower>();
  pRegistry.emplace<ReduceMaxLower>();
  pRegistry.emplace<ReduceMeanLower>();
  pRegistry.emplace<ReduceMinLower>();
  pRegistry.emplace<ReduceProdLower>();
  pRegistry.emplace<ReduceSumLower>();
  pRegistry.emplace<ReduceSumSquareLower>();
  pRegistry.emplace<ReluLower>();
  pRegistry.emplace<ReshapeLower>();
  // TODO: pRegistry.emplace<RNNLower>();
  pRegistry.emplace<ScaleLower>();
  pRegistry.emplace<ScaledTanhLower>();
  // TODO: pRegistry.emplace<ScanLower>();
  pRegistry.emplace<SeluLower>();
  pRegistry.emplace<ShapeLower>();
  pRegistry.emplace<SigmoidLower>();
  pRegistry.emplace<SinLower>();
  pRegistry.emplace<SizeLower>();
  pRegistry.emplace<SliceLower>();
  pRegistry.emplace<SoftmaxLower>();
  // TODO: pRegistry.emplace<SoftplusLower>();
  pRegistry.emplace<SoftsignLower>();
  pRegistry.emplace<SpaceToDepthLower>();
  pRegistry.emplace<SplitLower>();
  pRegistry.emplace<SqrtLower>();
  pRegistry.emplace<SqueezeLower>();
  pRegistry.emplace<SubLower>();
  pRegistry.emplace<SumLower>();
  pRegistry.emplace<TanLower>();
  pRegistry.emplace<TanhLower>();
  pRegistry.emplace<ThresholdedReluLower>();
  pRegistry.emplace<TileLower>();
  pRegistry.emplace<TopKLower>();
  pRegistry.emplace<TransposeLower>();
  pRegistry.emplace<UnsqueezeLower>();
  pRegistry.emplace<UpsampleLower>();
  pRegistry.emplace<XorLower>();
}


//===----------------------------------------------------------------------===//
// Non member functions
//===----------------------------------------------------------------------===//
TargetBackend* CreateCLangBackend(const TargetOptions& pOptions)
{
  return new CLangBackend(pOptions);
}

extern "C" void InitializeCLangONNCBackend()
{
  onnc::TargetRegistry::RegisterTargetBackend(getTheCLangTarget(),
      CreateCLangBackend);
}

