//===- NvDlaBackend.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaBackend.h"

#include "CodeEmitVisitor.h"
#include "NvDlaCalibrateAveragePoolResultPass.h"
#include "NvDlaCollectReshapeInfoPass.h"
#include "NvDlaFileGenPass.h"
#include "NvDlaIdentifyShufflePass.h"
#include "NvDlaMemInfoPass.h"
#include "NvDlaTaskSubmitPass.h"
#include "NvDlaUtil.h"
#include "SplitGroupConvPass.h"
#include "TargetInfo/NvDlaTargetInfo.h"
#include "TargetInfo/NvDlaTargetMemInfo.h"

#include <onnc/Analysis/NodeIRScheduler.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/CodeGen/BuildMemOperand.h>
#include <onnc/CodeGen/LinearScanMemAlloc.h>
#include <onnc/CodeGen/LiveIntervals.h>
#include <onnc/CodeGen/LiveValueMatrix.h>
#include <onnc/CodeGen/SetMemOperand.h>
#include <onnc/CodeGen/SlotIndexes.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/Support/Algorithm.h>
#include <onnc/Target/TargetRegistry.h>
#include <onnc/Target/TargetStandardPasses.h>
#include <onnc/Transforms/BookONNXGraphs.h>
#include <onnc/Transforms/BuildInitializers.h>
#include <onnc/Transforms/BuildInputOperators.h>
#include <onnc/Transforms/BuildOutputOperators.h>
#include <onnc/Transforms/DeadNodeElimination.h>
#include <onnc/Transforms/OnnxOptPass.h>
#include <onnc/Transforms/Optimizations/DivideGlobalAPIntoAPs.h>
#include <onnc/Transforms/Optimizations/EliminateIdentity.h>
#include <onnc/Transforms/Optimizations/PropagateConstWithDiffShape.h>
#include <onnc/Transforms/Optimizations/ReplaceGemmByConv.h>
#include <onnc/Transforms/Optimizations/SplitConvPass.h>
#include <onnc/Transforms/RemoveTrainingNodes.h>
#include <onnc/Transforms/TensorSel.h>
#include <onnc/Transforms/TensorSel/Standards/AddLower.h>
#include <onnc/Transforms/TensorSel/Standards/AveragePoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/BatchNormalizationLower.h>
#include <onnc/Transforms/TensorSel/Standards/CastLower.h>
#include <onnc/Transforms/TensorSel/Standards/ConcatLower.h>
#include <onnc/Transforms/TensorSel/Standards/ConvLower.h>
#include <onnc/Transforms/TensorSel/Standards/FlattenLower.h>
#include <onnc/Transforms/TensorSel/Standards/GemmLower.h>
#include <onnc/Transforms/TensorSel/Standards/IdentityLower.h>
#include <onnc/Transforms/TensorSel/Standards/GlobalAveragePoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/LRNLower.h>
#include <onnc/Transforms/TensorSel/Standards/LeakyReluLower.h>
#include <onnc/Transforms/TensorSel/Standards/MaxPoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/MulLower.h>
#include <onnc/Transforms/TensorSel/Standards/PReluLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReluLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReshapeLower.h>
#include <onnc/Transforms/TensorSel/Standards/SoftmaxLower.h>
#include <onnc/Transforms/TensorSel/Standards/SplitLower.h>
#include <onnc/Transforms/TensorSel/Standards/SqueezeLower.h>
#include <onnc/Transforms/TensorSel/Standards/SumLower.h>
#include <onnc/Transforms/TensorSel/Standards/TransposeLower.h>
#include <onnc/Transforms/TensorSel/Standards/UnsqueezeLower.h>
#include <onnc/Transforms/TensorSel/Standards/UpsampleLower.h>
#include <onnc/Transforms/TensorSel/Standards/IdentityLower.h>

#include <functional>
#include <memory>
#include <type_traits>

namespace onnc {

//===----------------------------------------------------------------------===//
// NvDlaBackend
//===----------------------------------------------------------------------===//
const Version NvDlaBackend::LOADABLE_VERSION = Version(1, 1, 1);
const Version NvDlaBackend::BLOB_DLA_VERSION = Version(1, 2, 0);
const Version NvDlaBackend::BLOB_EMU_VERSION = Version(1, 2, 0);

NvDlaBackend::NvDlaBackend(const TargetOptions& pOptions)
  : TargetBackend{pOptions}
  , NvDlaConstants(
      getConfig(nvdla::ConfigSet::nv_full, nvdla::ExecutionMode::direct, false))
  , m_VerboseLevel(pOptions.getVerboseLevel())
  , m_pMeta(*this)
  , m_CodeEmitVisitor(*this, m_pMeta,
                      std::bind(&NvDlaBackend::getCbufAllocType, this, std::placeholders::_1, std::placeholders::_2,
                                std::placeholders::_3, std::placeholders::_4),
                      m_VerboseLevel)
{
  m_pMemInfo = std::make_unique<NvDlaTargetMemInfo>();
}

void NvDlaBackend::addTensorSel(PassManager& passManager)
{
  // Translate from ONNX graph IR into ONNC IR
  addStandardTensorSel(passManager, *this);
}

CbufAllocType NvDlaBackend::getCbufAllocType(const NvDlaCubeInfo& xinfo, const NvDlaCubeInfo& winfo,
                                             Tensor::Dimension yDilation, unsigned& minNumNeededDataBanks)
{
  const unsigned input_height  = xinfo.dim_h;
  const unsigned kernel_height = winfo.dim_h;
  const unsigned kernel_width  = winfo.dim_w;
  minNumNeededDataBanks        = DIV_ROUNDUP(
    (xinfo.eps * min(kernel_height + (kernel_height - 1) * (yDilation - 1), input_height)), CBUF_BANK_DEPTH);

  if ((xinfo.banks + winfo.getBanksForFullWeights()) <= CBUF_BANK_NUM) {
    // Full data and full weights
    return CbufAllocType::kFullDataFullWeight;
  } else if ((xinfo.banks + winfo.getBanksForPartialWeights()) <= CBUF_BANK_NUM) {
    // Full data and partial weights
    return CbufAllocType::kFullDataPartialWeight;
  } else if ((minNumNeededDataBanks + winfo.getBanksForFullWeights()) <= CBUF_BANK_NUM) {
    // Split data and full weights
    return CbufAllocType::kSplitDataFullWeight;
  } else if ((minNumNeededDataBanks + winfo.getBanksForPartialWeights()) <= CBUF_BANK_NUM) {
    // Split data and partial weights
    return CbufAllocType::kSplitDataPartialWeight;
  } else if ((xinfo.banks + winfo.getBanksForMinimumWeights()) <= CBUF_BANK_NUM) {
    // Full data and minimum weights
    return CbufAllocType::kFullDataMinimumWeight;
  } else if ((minNumNeededDataBanks + winfo.getBanksForMinimumWeights()) <= CBUF_BANK_NUM) {
    // Split data and minimum weights
    return CbufAllocType::kSplitDataMinimumWeight;
  }

  return CbufAllocType::kUnfeasible;
}

void NvDlaBackend::addOnncIrOptimization(PassManager& passManager, OptimizationOptions& options)
{
  options.defaultEnable(OptimizationOption::divide_globalap_into_aps);
  options.defaultEnable(OptimizationOption::propagate_const_with_diff_shape);
  options.defaultEnable(OptimizationOption::expand_batch_normalization);
  options.defaultEnable(OptimizationOption::replace_gemm_by_conv);
  options.defaultEnable(OptimizationOption::eliminate_identity);

  TargetBackend::addOnncIrOptimization(passManager, options);

  if (options.isEnabled(OptimizationOption::split_conv_by_channel, true)) {
    passManager.add<SplitConvPass>(std::bind(&NvDlaBackend::getMaxNumOfConvChannels, this, std::placeholders::_1));
  }

  passManager
    .add<EliminateIdentity>()
    .add<NvDlaCalibrateAveragePoolResultPass>()
    .add<NvDlaIdentifyShufflePass>()
    .add<SplitGroupConvPass>()
    .add<NvDlaCollectReshapeInfoPass>(m_pMeta)
    ;
}

void NvDlaBackend::addTensorSched(PassManager& passManager)
{
  // After method AddTensorSel, operators have been scheduled in an
  // topological order, which totally respects the data dependency.
  // However, that might not be an optimized order for certain objective.
  // Add a scheduling optimization pass here.
}

void NvDlaBackend::addMemAlloc(PassManager& passManager)
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

  const NvDlaConstants& constants = *this;
  passManager.add<NvDlaMemInfoPass>(constants, &m_pMeta);
}

void NvDlaBackend::addCodeEmit(PassManager& passManager, const Path& pOutput)
{
  passManager
    .add<CodeEmitVisitorPass>(m_CodeEmitVisitor)
    .add<NvDlaTaskSubmitPass>(&m_pMeta, BLOB_DLA_VERSION, BLOB_EMU_VERSION)
    .add<NvDlaFileGenPass>(&m_pMeta, LOADABLE_VERSION)
    ;
}

void NvDlaBackend::RegisterLowers(LowerRegistry& pRegistry) const
{
  // CONV
  pRegistry.emplace<ConvLower>();
  pRegistry.emplace<GemmLower>();

  // SDP
  pRegistry.emplace<AddLower>();
  pRegistry.emplace<BatchNormalizationLower>();
  pRegistry.emplace<MulLower>();
  pRegistry.emplace<ReluLower>();
  pRegistry.emplace<SumLower>();

  // PDP
  pRegistry.emplace<MaxPoolLower>();
  pRegistry.emplace<AveragePoolLower>();
  pRegistry.emplace<GlobalAveragePoolLower>();

  // CDP
  pRegistry.emplace<LRNLower>();

  // RUBIK
  pRegistry.emplace<ReshapeLower>();

  // EMU
  pRegistry.emplace<SoftmaxLower>();

  pRegistry.emplace<ConcatLower>();
  pRegistry.emplace<FlattenLower>();
  pRegistry.emplace<IdentityLower>();
  pRegistry.emplace<SqueezeLower>();
  pRegistry.emplace<TransposeLower>();
  pRegistry.emplace<UnsqueezeLower>();
  
  pRegistry.emplace<IdentityLower>();
}

Tensor::Dimension NvDlaBackend::getMaxNumOfConvChannels(const Conv& conv)
{
  const NvDlaDims inputDims(*conv.getX());
  const NvDlaDims kernelDims(*conv.getW());

  const IntsAttr::VectorType& dilations = conv.getDilations().vector();
  const Tensor::Dimension     yDilation = static_cast<Tensor::Dimension>(2 <= size(dilations) ? dilations[1] : 1);

  const IntsAttr::VectorType& pads     = conv.getPads().vector();
  const IntsAttr::ValueType   padLeft  = (2 <= size(pads) ? pads[1] : 0);
  const IntsAttr::ValueType   padRight = (4 <= size(pads) ? pads[3] : 0);

  // try if whole data & weight can be placed in CBUF
  const NvDlaCubeInfo inputCube(*this, NVDLA_CUBE_FEATURE, inputDims.n, inputDims.c, inputDims.h, inputDims.w, padLeft,
                                padRight);
  const NvDlaCubeInfo kernelCube(*this, NVDLA_CUBE_WEIGHT, kernelDims.n, kernelDims.c, kernelDims.h, kernelDims.w);

  unsigned            dummy;
  const CbufAllocType allocType = getCbufAllocType(inputCube, kernelCube, yDilation, dummy);
  if (allocType != CbufAllocType::kUnfeasible) {
    return inputDims.c;
  }

  // reduce channel repeatly (by MAC_ATOMIC_K) and try if data & weight can still be placed in CBUF
  Tensor::Dimension maxChannel = inputDims.c - (inputDims.c % MAC_ATOMIC_K);
  assert(maxChannel % MAC_ATOMIC_K == 0);

  for (; 0 < maxChannel; maxChannel -= MAC_ATOMIC_K) {
    const NvDlaCubeInfo inputCube(*this, NVDLA_CUBE_FEATURE, inputDims.n, maxChannel, inputDims.h, inputDims.w);
    const NvDlaCubeInfo kernelCube(*this, NVDLA_CUBE_WEIGHT, kernelDims.n, maxChannel, kernelDims.h, kernelDims.w);

    const CbufAllocType allocType = getCbufAllocType(inputCube, kernelCube, yDilation, dummy);
    if (allocType != CbufAllocType::kUnfeasible) {
      return min(maxChannel, 8192);
    }
  }

  return Tensor::Dimension(-1);
}

} // namespace onnc

//===----------------------------------------------------------------------===//
// Non member functions
//===----------------------------------------------------------------------===//
onnc::TargetBackend* CreateNvDlaBackend(const onnc::TargetOptions& pOptions)
{
  return new onnc::NvDlaBackend(pOptions);
}

extern "C" void InitializeNvDlaONNCBackend()
{
  onnc::TargetRegistry::RegisterTargetBackend(onnc::getTheNvDlaTarget(), CreateNvDlaBackend);
}
