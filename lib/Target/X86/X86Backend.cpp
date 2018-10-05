//===- X86Backend.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "X86Backend.h"
#include "X86FuseConvRelu.h"
#include "X86InplaceValueFusible.h"
#include "X86RemoveWeightFromLiveIntervals.h"
#include "TargetInfo/X86TargetInfo.h"
#include "TargetInfo/X86TargetMemInfo.h"
#include <onnc/CodeGen/FuseInplaceValue.h>
#include <onnc/Target/TargetRegistry.h>
#include <onnc/Target/TargetStandardPasses.h>
#include <onnc/Transforms/TensorSel/LowerRegistry.h>
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
  // X86 only uses the standard ONNC IR and standard Lower, so just use the
  // standard Tensor selection passes.
  addStandardTensorSel(pPM, *this);

  pPM.add(CreateX86FuseConvReluPass());
}

void X86Backend::addMemAlloc(PassManager& pPM)
{
  // Fuse inplace value pairs before liveness analysis, because this pass may
  // delete values. ONNC IR graph topology may become invalid after this pass.
  pPM.add(CreateFuseInplaceValuePass(x86::IsInplaceValueFusible));

  // Input: Module
  // Output: LiveIntervals
  addStandardCreateLiveIntervals(pPM);

  // FIXME: Remove 'X86RemoveWeightFromLiveIntervals' pass, add configure in
  //        LiveIntervals to config this behaviour.
  pPM.add(CreateX86RemoveWeightFromLiveIntervalsPass());

  // Input: LiveIntervals
  // Output: MemAllocs
  addStandardMemoryAllocation(pPM, *this);

  // Input: MemAllocs
  // Output: Virtual memory address for each memory operands.
  addStandardSetMemOperands(pPM);
}

void X86Backend::addCodeEmit(PassManager& pPM, const Path& pOutput)
{
  // TODO
}

void X86Backend::RegisterLowers(LowerRegistry& pRegistry) const
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
