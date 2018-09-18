//===- MemAllocTest.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/StringList.h>
#include <onnc/CodeGen/BuildMemOperand.h>
#include <onnc/CodeGen/LinearScanMemAlloc.h>
#include <onnc/CodeGen/LiveIntervals.h>
#include <onnc/CodeGen/LiveValueMatrix.h>
#include <onnc/CodeGen/SlotIndexes.h>
#include <onnc/Core/AnalysisResolver.h>
#include <onnc/Core/InitializePasses.h>
#include <onnc/Core/PassManager.h>
#include <onnc/IR/IRBuilder.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Gemm.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/LRN.h>
#include <onnc/IR/Compute/MaxPool.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Softmax.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/OStrStream.h>
#include <onnc/Target/TargetBackend.h>
#include <onnc/Target/TargetMemInfo.h>
#include <skypat/skypat.h>
#include "../../lib/Target/X86/X86RemoveWeightFromLiveIntervals.h"

using namespace onnc;

//===----------------------------------------------------------------------===//
// Create Compute Graph Helper
//===----------------------------------------------------------------------===//
template<typename TensorTy>
static Tensor* CreateComputeTensor(ComputeGraph& pCG, const StringRef& pName,
                                   const Tensor::Dimensions& pDims)
{
  Tensor* t = pCG.addValue<TensorTy>(pName);
  t->setDimensions(pDims);
  return t;
}

static Tensor*
CreateFloatComputeTensor(ComputeGraph& pCG, const StringRef& pName,
                         const Tensor::Dimensions& pDims)
{
  return CreateComputeTensor<FloatTensor>(pCG, pName, pDims);
}

template<typename TensorTy>
static void CreateWeightOperator(ComputeGraph& pCG, const std::string& pName,
                                 const Tensor::Dimensions& pDims)
{
  Initializer* init = pCG.addOperator<Initializer>(pName);
  Tensor* value = CreateComputeTensor<TensorTy>(pCG, pName, pDims);
  init->setTensor(*value);
}

static void
CreateFloatWeightOperator(ComputeGraph& pCG, const std::string& pName,
                          const Tensor::Dimensions& pDims)
{
  CreateWeightOperator<FloatTensor>(pCG, pName, pDims);
}

template<typename OpTy, typename ... NodeCtorParams>
static OpTy* CreateComputeOperator(ComputeGraph& pCG,
                                   const StringList& pInputNames,
                                   NodeCtorParams&& ... pParams)
{
  OpTy* op = pCG.addOperator<OpTy>(pParams...);
  for (auto& iname : pInputNames)
    op->addInput(*pCG.getValue<Tensor>(iname));
  return op;
}

template<typename VTy>
static std::vector<VTy> GetValues(const std::vector<VTy>& pVec)
{
  return pVec;
}

//===----------------------------------------------------------------------===//
// Create Model
//===----------------------------------------------------------------------===//
static ComputeGraph& CreateAlexNet(Module& pM)
{
  IRBuilder builder(pM);

  ComputeGraph& cg = *builder.CreateComputeGraph("Alexnet");

  // Create Input.
  cg.addOperator<InputOperator>()->setTensor(
    *CreateFloatComputeTensor(cg, "data_0", {10, 3, 277, 277}));

  // Create weights.
  CreateFloatWeightOperator(cg, "conv1_w_0", {96, 3, 11, 11});
  CreateFloatWeightOperator(cg, "conv1_b_0", {96});
  CreateFloatWeightOperator(cg, "conv2_w_0", {256, 48, 5, 5});
  CreateFloatWeightOperator(cg, "conv2_b_0", {256});
  CreateFloatWeightOperator(cg, "conv3_w_0", {384, 256, 3, 3});
  CreateFloatWeightOperator(cg, "conv3_b_0", {384});
  CreateFloatWeightOperator(cg, "conv4_w_0", {384, 192, 3, 3});
  CreateFloatWeightOperator(cg, "conv4_b_0", {384});
  CreateFloatWeightOperator(cg, "conv5_w_0", {256, 192, 3, 3});
  CreateFloatWeightOperator(cg, "conv5_b_0", {256});
  CreateFloatWeightOperator(cg, "fc6_w_0", {4096, 9216});
  CreateFloatWeightOperator(cg, "fc6_b_0", {4096});
  CreateFloatWeightOperator(cg, "fc7_w_0", {4096, 4096});
  CreateFloatWeightOperator(cg, "fc7_b_0", {4096});
  CreateFloatWeightOperator(cg, "fc8_w_0", {1000, 4096});
  CreateFloatWeightOperator(cg, "fc8_b_0", {1000});
  CreateWeightOperator<Int64Tensor>(cg, "OC2_DUMMY_1", {2});

  // create nodes (layers)
  CreateComputeOperator<Conv>(cg,    {"data_0", "conv1_w_0", "conv1_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "conv1_1", {10, 96, 55, 55}));

  CreateComputeOperator<Relu>(cg,    {"conv1_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "relu1_1", {10, 96, 55, 55}));

  CreateComputeOperator<LRN>(cg,     {"relu1_1"}, /* size */ IntAttr(5))
    ->addOutput(*CreateFloatComputeTensor(cg, "norm1_1", {10, 96, 55, 55}));

  CreateComputeOperator<MaxPool>(cg, {"norm1_1"},
                                 /* krnl shape */ GetValues<int64_t>({3, 3}))
    ->addOutput(*CreateFloatComputeTensor(cg, "pool1_1", {10, 96, 27, 27}));

  CreateComputeOperator<Conv>(cg,    {"pool1_1", "conv2_w_0", "conv2_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "conv2_1", {10, 256, 27, 27}));

  CreateComputeOperator<Relu>(cg,    {"conv2_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "relu2_1", {10, 256, 27, 27}));

  CreateComputeOperator<LRN>(cg,     {"relu2_1"}, IntAttr(1))
    ->addOutput(*CreateFloatComputeTensor(cg, "norm2_1", {10, 256, 27, 27}));

  CreateComputeOperator<MaxPool>(cg, {"norm2_1"},
                                 /* krnl shape */ GetValues<int64_t>({3, 3}))
    ->addOutput(*CreateFloatComputeTensor(cg, "pool2_1", {10, 256, 13, 13}));

  CreateComputeOperator<Conv>(cg,    {"pool2_1", "conv3_w_0", "conv3_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "conv3_1", {10, 384, 13, 13}));

  CreateComputeOperator<Relu>(cg,    {"conv3_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "relu3_1", {10, 384, 13, 13}));

  CreateComputeOperator<Conv>(cg,    {"relu3_1", "conv4_w_0", "conv4_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "conv4_1", {10, 384, 13, 13}));

  CreateComputeOperator<Relu>(cg,    {"conv4_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "relu4_1", {10, 384, 13, 13}));

  CreateComputeOperator<Conv>(cg,    {"relu4_1", "conv5_w_0", "conv5_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "conv5_1", {10, 256, 13, 13}));

  CreateComputeOperator<Relu>(cg,    {"conv5_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "relu5_1", {10, 256, 13, 13}));

  CreateComputeOperator<MaxPool>(cg, {"relu5_1"},
                                 /* krnl shape */ GetValues<int64_t>({3, 3}))
    ->addOutput(*CreateFloatComputeTensor(cg, "pool5_1", {10, 256, 6, 6}));

  CreateComputeOperator<Reshape>(cg,    {"pool5_1", "OC2_DUMMY_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "OC2_DUMMY_0", {10, 9216}));

  CreateComputeOperator<Gemm>(cg,    {"OC2_DUMMY_0", "fc6_w_0", "fc6_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "fc6_1", {10, 4096}));

  CreateComputeOperator<Relu>(cg,    {"fc6_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "relu6_1", {10, 4096}));

  CreateComputeOperator<Gemm>(cg,    {"relu6_1", "fc7_w_0", "fc7_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "fc7_1", {10, 4096}));

  CreateComputeOperator<Relu>(cg,    {"fc7_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "relu7_1", {10, 4096}));

  CreateComputeOperator<Gemm>(cg,    {"relu7_1", "fc8_w_0", "fc8_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "fc8_1", {10, 1000}));

  CreateComputeOperator<Softmax>(cg, {"fc8_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "prob_1", {10, 1000}));

  CreateComputeOperator<OutputOperator>(cg, {"prob_1"});

  return cg;
}

//===----------------------------------------------------------------------===//
// Virtual Target
//===----------------------------------------------------------------------===//
class VTargetMemInfo : public TargetMemInfo
{
public:
  MemSize getTensorMemorySize(const Tensor& pVal) override {
    uint64_t align, size;
    switch (pVal.kind()) {
    case kUint8:
    case kInt8:
    case kBoolean:
      align = 16, size = 1;
      break;

    case kUint16:
    case kInt16:
    case kFloat16:
      align = 16, size = 2;
      break;

    case kFloat:
    case kInt32:
    case kUint32:
      align = 16, size = 4;
      break;

    case kInt64:
    case kUint64:
      align = 16, size = 8;
      break;

    default:
      assert(false && "Un-support value type.");
      align = 16, size = 4;
    }

    for (auto i : pVal.getDimensions())
      size *= i;

    return MemSize(align, size);
  }
};

class VTargetBackend : public TargetBackend
{
public:
  VTargetBackend(const TargetOptions& pOptions)
    : TargetBackend(pOptions) {
    m_pMemInfo = &m_VTMI;
  }

  VTargetMemInfo m_VTMI;
};

//===----------------------------------------------------------------------===//
// Testcases
//===----------------------------------------------------------------------===//
SKYPAT_F(MemAllocTest, live_interval_test)
{
  Module module;
  ComputeGraph& cg = CreateAlexNet(module);

  PassManager passMgr;
  AnalysisResolver* resolver = nullptr;

  BuildSlotIndexes buildSlotIdx;
  buildSlotIdx.runOnModule(module);

  LiveIntervals liveIntrvls;
  resolver = new AnalysisResolver(passMgr);
  resolver->add(buildSlotIdx.getPassID(), buildSlotIdx);
  liveIntrvls.setResolver(*resolver);
  liveIntrvls.runOnModule(module);

  // Answer:
  struct ValRange
  {
    const char* valName;
    int start, end;
    ValRange(const char* pName, int start, int end)
      : valName(pName), start(start), end(end) {
    }
  };

  const ValRange valRanges[] = {
    {"data_0", 0, 18},
    {"conv1_w_0", 1, 18}, {"conv1_b_0", 2, 18},
    {"conv2_w_0", 3, 22}, {"conv2_b_0", 4, 22},
    {"conv3_w_0", 5, 26}, {"conv3_b_0", 6, 26},
    {"conv4_w_0", 7, 28}, {"conv4_b_0", 8, 28},
    {"conv5_w_0", 9, 30}, {"conv5_b_0", 10, 30},
    {"fc6_w_0", 11, 34}, {"fc6_b_0", 12, 34},
    {"fc7_w_0", 13, 36}, {"fc7_b_0", 14, 36},
    {"fc8_w_0", 15, 38}, {"fc8_b_0", 16, 38},
    {"OC2_DUMMY_1", 17, 33},
    {"conv1_1", 18, 19}, {"relu1_1", 19, 20},
    {"norm1_1", 20, 21}, {"pool1_1", 21, 22},
    {"conv2_1", 22, 23}, {"relu2_1", 23, 24},
    {"norm2_1", 24, 25}, {"pool2_1", 25, 26},
    {"conv3_1", 26, 27}, {"relu3_1", 27, 28},
    {"conv4_1", 28, 29}, {"relu4_1", 29, 30},
    {"conv5_1", 30, 31}, {"relu5_1", 31, 32},
    {"pool5_1", 32, 33},
    {"OC2_DUMMY_0", 33, 34},
    {"fc6_1", 34, 35}, {"relu6_1", 35, 36},
    {"fc7_1", 36, 37}, {"relu7_1", 37, 38},
    {"fc8_1", 38, 39}, {"prob_1", 39, 40}
  };

  for (auto &vrIter : valRanges) {
    const Value* v = cg.getValue(vrIter.valName);
    const LiveInterval* li = liveIntrvls.getInterval(v);
    ASSERT_TRUE(li->beginIndex().getIndex() == vrIter.start);
    ASSERT_TRUE(li->endIndex().getIndex() == vrIter.end);
  }
}

SKYPAT_F(MemAllocTest, live_matrix_test)
{
  Module module;
  CreateAlexNet(module);

  PassManager passMgr;
  AnalysisResolver* resolver = nullptr;

  BuildSlotIndexes buildSlotIdx;
  buildSlotIdx.runOnModule(module);

  LiveIntervals liveIntrvls;
  resolver = new AnalysisResolver(passMgr);
  resolver->add(buildSlotIdx.getPassID(), buildSlotIdx);
  liveIntrvls.setResolver(*resolver);
  liveIntrvls.runOnModule(module);

  LiveValueMatrix liveMat;
  resolver = new AnalysisResolver(passMgr);
  resolver->add(liveIntrvls.getPassID(), liveIntrvls);
  liveMat.setResolver(*resolver);
  liveMat.runOnModule(module);

  for (auto& outerIt : module.getValueList()) {
    Value* v = outerIt.value();
    const LiveInterval* vli = liveIntrvls.getInterval(v);
    std::vector<LiveInterval*> expectOverlaps;

    for (auto& innerIt : module.getValueList()) {
      Value* u = innerIt.value();
      if (v == u)
        continue;
      const LiveInterval* uli = liveIntrvls.getInterval(u);
      if (uli->overlap(*vli))
        expectOverlaps.push_back(const_cast<LiveInterval*>(uli));
    }
    std::sort(expectOverlaps.begin(), expectOverlaps.end());

    std::vector<LiveInterval*> overlaps =
      liveMat.getInterferingLiveIntervals(v);
    std::sort(overlaps.begin(), overlaps.end());

    ASSERT_TRUE(expectOverlaps == overlaps);
  }
}

SKYPAT_F(MemAllocTest, linear_mem_alloc_test)
{
  Module module;
  CreateAlexNet(module);

  PassManager passMgr;
  AnalysisResolver resolver(passMgr);
  AnalysisResolver* nullResolver = nullptr;

  TargetOptions opt;
  VTargetBackend vtarget(opt);

  BuildSlotIndexes buildSlotIdx;
  LiveIntervals liveIntrvls;
  LiveValueMatrix liveMat;
  LinearScanMemAlloc linearMemAlloc(&vtarget);
  BuildMemOperand buildMemOpnd;

  resolver.add(buildSlotIdx.getPassID(), buildSlotIdx);
  resolver.add(liveIntrvls.getPassID(), liveIntrvls);
  resolver.add(liveMat.getPassID(), liveMat);
  resolver.add(linearMemAlloc.getPassID(), linearMemAlloc);

  liveIntrvls.setResolver(resolver);
  liveMat.setResolver(resolver);
  linearMemAlloc.setResolver(resolver);

  buildSlotIdx.runOnModule(module);
  liveIntrvls.runOnModule(module);
  liveMat.runOnModule(module);
  buildMemOpnd.runOnModule(module);
  linearMemAlloc.runOnModule(module);

  for (auto li : liveIntrvls.getSortedIntervals()) {
    LinearScanMemAlloc::AllocEntry
      myAlloc = linearMemAlloc.getAlloc(li->getValue());

    for (auto overlappedLI : liveMat.getInterferingLiveIntervals(li))
    {
      LinearScanMemAlloc::AllocEntry otherAlloc =
        linearMemAlloc.getAlloc(overlappedLI->getValue());
      ASSERT_FALSE(otherAlloc.overlap(myAlloc));
    }
  }

  liveIntrvls.setResolver(*nullResolver);
  liveMat.setResolver(*nullResolver);
  linearMemAlloc.setResolver(*nullResolver);
}

SKYPAT_F(MemAllocTest, exclude_weight_linear_mem_alloc_test)
{
  TargetOptions opt;
  VTargetBackend vtarget(opt);

  BuildSlotIndexes* buildSlotIdx = new BuildSlotIndexes();
  LiveIntervals* liveIntrvls = new LiveIntervals();
  X86RemoveWeightFromLiveIntervals*
    rmWeight = new X86RemoveWeightFromLiveIntervals();
  LiveValueMatrix* liveMat = new LiveValueMatrix();
  LinearScanMemAlloc* linearMemAlloc = new LinearScanMemAlloc(&vtarget);
  BuildMemOperand* buildMemOpnd = new BuildMemOperand();

  PassManager passMgr;
  passMgr.add(buildSlotIdx);
  passMgr.add(liveIntrvls);
  passMgr.add(rmWeight);
  passMgr.add(liveMat);
  passMgr.add(buildMemOpnd);
  passMgr.add(linearMemAlloc);

  Module module;
  CreateAlexNet(module);

  passMgr.run(module);

  for (auto li : liveIntrvls->getSortedIntervals()) {
    LinearScanMemAlloc::AllocEntry
      myAlloc = linearMemAlloc->getAlloc(li->getValue());
    ComputeOperator* op = static_cast<ComputeOperator*>(li->getValue()
                                                          ->getDefine());
    ASSERT_FALSE(isa<Initializer>(op));

    for (auto overlappedLI : liveMat->getInterferingLiveIntervals(li))
    {
      LinearScanMemAlloc::AllocEntry otherAlloc =
        linearMemAlloc->getAlloc(overlappedLI->getValue());
      ASSERT_FALSE(otherAlloc.overlap(myAlloc));
    }
  }
}
