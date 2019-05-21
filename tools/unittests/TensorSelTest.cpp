//===- TensorSelTest.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/Transforms/BookONNXGraphs.h>
#include <onnc/Transforms/TensorSel.h>
#include <onnc/IR/IRBuilder.h>
#include <onnc/Core/PassManager.h>
#include <onnc/Transforms/DeadNodeElimination.h>
#include <onnc/Transforms/RemoveTrainingNodes.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Transforms/BuildInputOperators.h>
#include <onnc/Transforms/BuildInitializers.h>
#include <onnc/IRReader/ONNXReader.h>
#include <onnc/Transforms/BuildOutputOperators.h>
#include <onnc/ADT/Rope.h>
#include <onnc/Support/OFStream.h>

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
// Any Test
//===----------------------------------------------------------------------===//
SKYPAT_F(TensorSelTest, alexnet)
{
  Path path(TOPDIR);
  path.append("tools")
      .append("unittests")
      .append("data")
      .append("bvlc_alexnet")
      .append("model.onnx");
  onnc::Module module;
  onnc::onnx::Reader reader;
  SystemError err = reader.parse(path, module);
  ASSERT_TRUE(err.isGood());

  module.dump();

  PassRegistry registry;
  PassManager pm(registry);
  PassManager::State state;

  pm.add(CreateRemoveTrainingNodesPass(), state);
  pm.add(CreateUpdateGraphOutputSizePass(), state);
  pm.add(CreateDeadNodeEliminationPass(), state);
  pm.add(CreateBookONNXGraphs(), state);
  pm.add(CreateBuildInitializers(), state);
  pm.add(CreateBuildInputOperators(), state);

  /// create tensor selection
  TensorSel* tensor_selection = new TensorSel();
  tensor_selection->getLowerRegistry().emplace<AbsLower>();
  tensor_selection->getLowerRegistry().emplace<AcosLower>();
  tensor_selection->getLowerRegistry().emplace<AddLower>();
  tensor_selection->getLowerRegistry().emplace<AffineLower>();
  tensor_selection->getLowerRegistry().emplace<AndLower>();
  tensor_selection->getLowerRegistry().emplace<ArgMaxLower>();
  tensor_selection->getLowerRegistry().emplace<ArgMinLower>();
  tensor_selection->getLowerRegistry().emplace<AsinLower>();
  tensor_selection->getLowerRegistry().emplace<AtanLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<ATenLower>();
  tensor_selection->getLowerRegistry().emplace<AveragePoolLower>();
  tensor_selection->getLowerRegistry().emplace<BatchNormalizationLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<CastLower>();
  tensor_selection->getLowerRegistry().emplace<CeilLower>();
  tensor_selection->getLowerRegistry().emplace<ClipLower>();
  tensor_selection->getLowerRegistry().emplace<ConcatLower>();
  // FIXME: tensor_selection->getLowerRegistry().emplace<ConstantLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<ConstantFillLower>();
  tensor_selection->getLowerRegistry().emplace<ConvLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<ConvTransposeLower>();
  tensor_selection->getLowerRegistry().emplace<CosLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<CropLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<DepthToSpaceLower>();
  tensor_selection->getLowerRegistry().emplace<DivLower>();
  tensor_selection->getLowerRegistry().emplace<DropoutLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<EluLower>();
  tensor_selection->getLowerRegistry().emplace<EqualLower>();
  tensor_selection->getLowerRegistry().emplace<ExpLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<ExpandLower>();
  tensor_selection->getLowerRegistry().emplace<FlattenLower>();
  tensor_selection->getLowerRegistry().emplace<FloorLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<GatherLower>();
  tensor_selection->getLowerRegistry().emplace<GemmLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<GivenTensorFillLower>();
  tensor_selection->getLowerRegistry().emplace<GlobalAveragePoolLower>();
  tensor_selection->getLowerRegistry().emplace<GlobalLpPoolLower>();
  tensor_selection->getLowerRegistry().emplace<GlobalMaxPoolLower>();
  tensor_selection->getLowerRegistry().emplace<GreaterLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<GruLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<GruUnitLower>();
  tensor_selection->getLowerRegistry().emplace<HardmaxLower>();
  tensor_selection->getLowerRegistry().emplace<HardSigmoidLower>();
  tensor_selection->getLowerRegistry().emplace<IdentityLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<IfLower>();
  tensor_selection->getLowerRegistry().emplace<ImageScalerLower>();
  tensor_selection->getLowerRegistry().emplace<InstanceNormalizationLower>();
  tensor_selection->getLowerRegistry().emplace<LeakyReluLower>();
  tensor_selection->getLowerRegistry().emplace<LessLower>();
  tensor_selection->getLowerRegistry().emplace<LogLower>();
  tensor_selection->getLowerRegistry().emplace<LogSoftmaxLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<LoopLower>();
  tensor_selection->getLowerRegistry().emplace<LpNormalizationLower>();
  tensor_selection->getLowerRegistry().emplace<LpPoolLower>();
  tensor_selection->getLowerRegistry().emplace<LRNLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<LSTMLower>();
  tensor_selection->getLowerRegistry().emplace<MatMulLower>();
  tensor_selection->getLowerRegistry().emplace<MaxLower>();
  tensor_selection->getLowerRegistry().emplace<MaxPoolLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<MaxRoiPoolLower>();
  tensor_selection->getLowerRegistry().emplace<MeanLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<MeanVarianceNormalizationLower>();
  tensor_selection->getLowerRegistry().emplace<MinLower>();
  tensor_selection->getLowerRegistry().emplace<MulLower>();
  tensor_selection->getLowerRegistry().emplace<MultinomialLower>();
  tensor_selection->getLowerRegistry().emplace<NegLower>();
  tensor_selection->getLowerRegistry().emplace<NotLower>();
  tensor_selection->getLowerRegistry().emplace<OrLower>();
  tensor_selection->getLowerRegistry().emplace<PadLower>();
  tensor_selection->getLowerRegistry().emplace<ParametricSoftplusLower>();
  tensor_selection->getLowerRegistry().emplace<PowLower>();
  tensor_selection->getLowerRegistry().emplace<PReluLower>();
  tensor_selection->getLowerRegistry().emplace<RandomNormalLower>();
  tensor_selection->getLowerRegistry().emplace<RandomNormalLikeLower>();
  tensor_selection->getLowerRegistry().emplace<RandomUniformLower>();
  tensor_selection->getLowerRegistry().emplace<RandomUniformLikeLower>();
  tensor_selection->getLowerRegistry().emplace<ReciprocalLower>();
  tensor_selection->getLowerRegistry().emplace<ReduceL1Lower>();
  tensor_selection->getLowerRegistry().emplace<ReduceL2Lower>();
  tensor_selection->getLowerRegistry().emplace<ReduceLogSumLower>();
  tensor_selection->getLowerRegistry().emplace<ReduceLogSumExpLower>();
  tensor_selection->getLowerRegistry().emplace<ReduceMaxLower>();
  tensor_selection->getLowerRegistry().emplace<ReduceMeanLower>();
  tensor_selection->getLowerRegistry().emplace<ReduceMinLower>();
  tensor_selection->getLowerRegistry().emplace<ReduceProdLower>();
  tensor_selection->getLowerRegistry().emplace<ReduceSumLower>();
  tensor_selection->getLowerRegistry().emplace<ReduceSumSquareLower>();
  tensor_selection->getLowerRegistry().emplace<ReluLower>();
  tensor_selection->getLowerRegistry().emplace<ReshapeLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<RNNLower>();
  tensor_selection->getLowerRegistry().emplace<ScaleLower>();
  tensor_selection->getLowerRegistry().emplace<ScaledTanhLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<ScanLower>();
  tensor_selection->getLowerRegistry().emplace<SeluLower>();
  tensor_selection->getLowerRegistry().emplace<ShapeLower>();
  tensor_selection->getLowerRegistry().emplace<SigmoidLower>();
  tensor_selection->getLowerRegistry().emplace<SinLower>();
  tensor_selection->getLowerRegistry().emplace<SizeLower>();
  tensor_selection->getLowerRegistry().emplace<SliceLower>();
  tensor_selection->getLowerRegistry().emplace<SoftmaxLower>();
  // TODO: tensor_selection->getLowerRegistry().emplace<SoftplusLower>();
  tensor_selection->getLowerRegistry().emplace<SoftsignLower>();
  tensor_selection->getLowerRegistry().emplace<SpaceToDepthLower>();
  tensor_selection->getLowerRegistry().emplace<SplitLower>();
  tensor_selection->getLowerRegistry().emplace<SqrtLower>();
  tensor_selection->getLowerRegistry().emplace<SqueezeLower>();
  tensor_selection->getLowerRegistry().emplace<SubLower>();
  tensor_selection->getLowerRegistry().emplace<SumLower>();
  tensor_selection->getLowerRegistry().emplace<TanLower>();
  tensor_selection->getLowerRegistry().emplace<TanhLower>();
  tensor_selection->getLowerRegistry().emplace<ThresholdedReluLower>();
  tensor_selection->getLowerRegistry().emplace<TileLower>();
  tensor_selection->getLowerRegistry().emplace<TopKLower>();
  tensor_selection->getLowerRegistry().emplace<TransposeLower>();
  tensor_selection->getLowerRegistry().emplace<UnsqueezeLower>();
  tensor_selection->getLowerRegistry().emplace<UpsampleLower>();
  tensor_selection->getLowerRegistry().emplace<XorLower>();
  pm.add(tensor_selection, state);
  pm.add(CreateBuildOutputOperators(), state);

  // RemoveTrainingNodesPass
  int counter = 0;
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // UpdateGraphOutputSizePass
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // DeadNodeEliminationPass
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // BookONNXGraphs
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // BuildInitializers
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // BuildInputOperators
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // TensorSel 
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // BuildOutputOperators
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  //ASSERT_TRUE(state.execution.empty());
  //ASSERT_TRUE(state.changed);
  //int run_result = pm.run(module);
  //ASSERT_TRUE(EXIT_SUCCESS == run_result);
}
