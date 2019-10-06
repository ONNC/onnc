//===- SplitConvTest.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "GraphUtils.h"
#include "TestUtils.h"
#include <skypat/skypat.h>

#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Split.h>
#include <onnc/IR/Compute/Sum.h>
#include <onnc/Transforms/Optimizations/SplitConvPass.h>

namespace onnc {

//===----------------------------------------------------------------------===//
// Real Case
//===----------------------------------------------------------------------===//
static const int dimM = 2;
static void      createRealConv(Module& pM)
{
  ComputeGraph& cg = BuildGraph(pM, "real_conv");
  AddInput(cg, "A", {1, 256, 6, 6});
  AddInput(cg, "B", {dimM, 256, 6, 6});

  FloatTensor::ValueList bias(dimM, 0.2);
  CreateFloatWeightOperatorWithValues(cg, "C", {dimM}, bias);

  AddOperator<Conv>(cg, {"A", "B", "C"}, "Y", {1, dimM, 1, 1}, StringAttr("NOTSET"), GetInts({1, 1}), IntAttr(1),
                    GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));
  AddOutput(cg, {"Y"});
}

static void createRealSplitConvs50(Module& pM)
{
  ComputeGraph& cg = BuildGraph(pM, "real_split_convs_50");
  AddInput(cg, "A", {1, 256, 6, 6});
  AddInput(cg, "B", {dimM, 256, 6, 6});

  FloatTensor::ValueList bias(dimM, 0.2);
  CreateFloatWeightOperatorWithValues(cg, "C", {dimM}, bias);

  AddOperator<Split>(
    cg, {"A"},
    {"split_conv_s_0", "split_conv_s_2", "split_conv_s_4", "split_conv_s_6", "split_conv_s_8", "split_conv_s_10"},
    {{1, 50, 6, 6}, {1, 50, 6, 6}, {1, 50, 6, 6}, {1, 50, 6, 6}, {1, 50, 6, 6}, {1, 6, 6, 6}}, IntAttr(1),
    GetInts({50, 50, 50, 50, 50, 6}));
  AddOperator<Split>(
    cg, {"B"},
    {"split_conv_s_1", "split_conv_s_3", "split_conv_s_5", "split_conv_s_7", "split_conv_s_9", "split_conv_s_11"},
    {{dimM, 50, 6, 6}, {dimM, 50, 6, 6}, {dimM, 50, 6, 6}, {dimM, 50, 6, 6}, {dimM, 50, 6, 6}, {dimM, 6, 6, 6}},
    IntAttr(1), GetInts({50, 50, 50, 50, 50, 6}));

  AddOperator<Conv>(cg, {"split_conv_s_0", "split_conv_s_1", "C"}, "split_conv_c_0", {1, dimM, 1, 1},
                    StringAttr("NOTSET"), GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}),
                    GetInts({6, 6}));
  AddOperator<Conv>(cg, {"split_conv_s_2", "split_conv_s_3"}, "split_conv_c_1", {1, dimM, 1, 1}, StringAttr("NOTSET"),
                    GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));
  AddOperator<Conv>(cg, {"split_conv_s_4", "split_conv_s_5"}, "split_conv_c_2", {1, dimM, 1, 1}, StringAttr("NOTSET"),
                    GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));
  AddOperator<Conv>(cg, {"split_conv_s_6", "split_conv_s_7"}, "split_conv_c_3", {1, dimM, 1, 1}, StringAttr("NOTSET"),
                    GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));
  AddOperator<Conv>(cg, {"split_conv_s_8", "split_conv_s_9"}, "split_conv_c_4", {1, dimM, 1, 1}, StringAttr("NOTSET"),
                    GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));
  AddOperator<Conv>(cg, {"split_conv_s_10", "split_conv_s_11"}, "split_conv_c_5", {1, dimM, 1, 1}, StringAttr("NOTSET"),
                    GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));

  AddOperator<Sum>(
    cg, {"split_conv_c_0", "split_conv_c_1", "split_conv_c_2", "split_conv_c_3", "split_conv_c_4", "split_conv_c_5"},
    "Y", {1, dimM, 1, 1});

  AddOutput(cg, {"Y"});
}

static void createRealSplitConvs128(Module& pM)
{
  ComputeGraph& cg = BuildGraph(pM, "real_split_convs_128");
  AddInput(cg, "A", {1, 256, 6, 6});
  AddInput(cg, "B", {dimM, 256, 6, 6});

  FloatTensor::ValueList bias(dimM, 0.2);
  CreateFloatWeightOperatorWithValues(cg, "C", {dimM}, bias);

  AddOperator<Split>(cg, {"A"}, {"split_conv_s_0", "split_conv_s_2"}, {{1, 128, 6, 6}, {1, 128, 6, 6}}, IntAttr(1),
                     GetInts({128, 128}));
  AddOperator<Split>(cg, {"B"}, {"split_conv_s_1", "split_conv_s_3"}, {{dimM, 128, 6, 6}, {dimM, 128, 6, 6}},
                     IntAttr(1), GetInts({128, 128}));

  AddOperator<Conv>(cg, {"split_conv_s_0", "split_conv_s_1", "C"}, "split_conv_c_0", {1, dimM, 1, 1},
                    StringAttr("NOTSET"), GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}),
                    GetInts({6, 6}));
  AddOperator<Conv>(cg, {"split_conv_s_2", "split_conv_s_3"}, "split_conv_c_1", {1, dimM, 1, 1}, StringAttr("NOTSET"),
                    GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));

  AddOperator<Sum>(cg, {"split_conv_c_0", "split_conv_c_1"}, "Y", {1, dimM, 1, 1});

  AddOutput(cg, {"Y"});
}

static Tensor::Dimension mockGetMaxNumOfConvChannels(const Conv& conv, Tensor::Dimension maxNumOfInputChannels,
                                                     Tensor::Dimension atomicNumOfChannels)
{
  assert(2 <= conv.getX()->getNumOfDimensions());

  const auto isConvDoable = [=](Tensor::Dimension numOfInputChannels) -> bool {
    return numOfInputChannels <= maxNumOfInputChannels;
  };

  Tensor::Dimension numOfInputChannels = conv.getX()->dimension(1);

  assert(numOfInputChannels >= 1 && atomicNumOfChannels >= 1);

  Tensor::Dimension left  = 1;
  Tensor::Dimension right = numOfInputChannels + atomicNumOfChannels + 1;
  Tensor::Dimension mid   = 0;
  // Use binary search to get max number of Conv channels
  // left and right will stop at max num + 1
  while (left < right) {
    mid                = (right - left) / 2 + left;
    numOfInputChannels = mid;
    if (isConvDoable(numOfInputChannels)) {
      left = mid + 1;
    } else {
      right = mid;
    }
  }
  assert(1 <= left);

  const Tensor::Dimension ans = left - 1;
  return ans < atomicNumOfChannels ? -1 : ans - (ans % atomicNumOfChannels);
}

// This one needs a lot of time to run if set dimM = 4096 ...
SKYPAT_F(NvDlaSplitConv, real_case_50)
{
  SplitConvPass::resetIndices();
  std::string ans = getNetworkString(createRealSplitConvs50);
  testOptPassOnNetwork<SplitConvPass>(createRealConv, Pass::kModuleChanged, ans,
                                      std::bind(mockGetMaxNumOfConvChannels, std::placeholders::_1, 50, 10));
}

SKYPAT_F(NvDlaSplitConv, real_case_128)
{
  SplitConvPass::resetIndices();
  std::string ans = getNetworkString(createRealSplitConvs128);
  testOptPassOnNetwork<SplitConvPass>(createRealConv, Pass::kModuleChanged, ans,
                                      std::bind(mockGetMaxNumOfConvChannels, std::placeholders::_1, 128, 8));
}

//===----------------------------------------------------------------------===//
// Real Case with Constant(Initializer) Kernels
//===----------------------------------------------------------------------===//
static void createRealConv_const(Module& pM)
{
  ComputeGraph& cg = BuildGraph(pM, "real_conv");
  AddInput(cg, "A", {1, 256, 6, 6});

  FloatTensor::ValueList weights(dimM * 9216, 0.9), bias(dimM, 0.2);
  for (FloatTensor::Size idx = 0; idx < weights.size(); ++idx)
    weights[idx] *= (idx + 1);

  CreateFloatWeightOperatorWithValues(cg, "B", {dimM, 256, 6, 6}, weights);
  CreateFloatWeightOperatorWithValues(cg, "C", {dimM}, bias);

  AddOperator<Conv>(cg, {"A", "B", "C"}, "Y", {1, dimM, 1, 1}, StringAttr("NOTSET"), GetInts({1, 1}), IntAttr(1),
                    GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));
  AddOutput(cg, {"Y"});
}

static void createRealSplitConvs50_const(Module& pM)
{
  ComputeGraph& cg = BuildGraph(pM, "real_split_convs_50");
  AddInput(cg, "A", {1, 256, 6, 6});

  FloatTensor::ValueList bias(dimM, 0.2);
  CreateFloatWeightOperatorWithValues(cg, "C", {dimM}, bias);

  AddOperator<Split>(
    cg, {"A"},
    {"split_conv_s_0", "split_conv_s_2", "split_conv_s_4", "split_conv_s_6", "split_conv_s_8", "split_conv_s_10"},
    {{1, 50, 6, 6}, {1, 50, 6, 6}, {1, 50, 6, 6}, {1, 50, 6, 6}, {1, 50, 6, 6}, {1, 6, 6, 6}}, IntAttr(1),
    GetInts({50, 50, 50, 50, 50, 6}));

  FloatTensor::ValueList weights(dimM * 9216, 0.9);
  for (FloatTensor::Size idx = 0; idx < weights.size(); ++idx)
    weights[idx] *= (idx + 1);

  FloatTensor::ValueList weightss[6];
  for (int i = 0; i < 6; ++i) {
    if (i != 5)
      weightss[i].reserve(dimM * 50 * 6 * 6);
    else
      weightss[i].reserve(dimM * 6 * 6 * 6);
  }

  for (int a = 0; a < dimM; ++a)
    for (int b = 0; b < 256; ++b)
      for (int c = 0; c < 36; ++c) {
        weightss[b / 50].emplace_back(weights[a * 9216 + b * 36 + c]);
      }

  CreateFloatWeightOperatorWithValues(cg, "split_conv_s_1", {dimM, 50, 6, 6}, weightss[0]);
  AddOperator<Conv>(cg, {"split_conv_s_0", "split_conv_s_1", "C"}, "split_conv_c_0", {1, dimM, 1, 1},
                    StringAttr("NOTSET"), GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}),
                    GetInts({6, 6}));

  CreateFloatWeightOperatorWithValues(cg, "split_conv_s_3", {dimM, 50, 6, 6}, weightss[1]);
  AddOperator<Conv>(cg, {"split_conv_s_2", "split_conv_s_3"}, "split_conv_c_1", {1, dimM, 1, 1}, StringAttr("NOTSET"),
                    GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));

  CreateFloatWeightOperatorWithValues(cg, "split_conv_s_5", {dimM, 50, 6, 6}, weightss[2]);
  AddOperator<Conv>(cg, {"split_conv_s_4", "split_conv_s_5"}, "split_conv_c_2", {1, dimM, 1, 1}, StringAttr("NOTSET"),
                    GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));

  CreateFloatWeightOperatorWithValues(cg, "split_conv_s_7", {dimM, 50, 6, 6}, weightss[3]);
  AddOperator<Conv>(cg, {"split_conv_s_6", "split_conv_s_7"}, "split_conv_c_3", {1, dimM, 1, 1}, StringAttr("NOTSET"),
                    GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));

  CreateFloatWeightOperatorWithValues(cg, "split_conv_s_9", {dimM, 50, 6, 6}, weightss[4]);
  AddOperator<Conv>(cg, {"split_conv_s_8", "split_conv_s_9"}, "split_conv_c_4", {1, dimM, 1, 1}, StringAttr("NOTSET"),
                    GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));

  CreateFloatWeightOperatorWithValues(cg, "split_conv_s_11", {dimM, 6, 6, 6}, weightss[5]);
  AddOperator<Conv>(cg, {"split_conv_s_10", "split_conv_s_11"}, "split_conv_c_5", {1, dimM, 1, 1}, StringAttr("NOTSET"),
                    GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));

  AddOperator<Sum>(
    cg, {"split_conv_c_0", "split_conv_c_1", "split_conv_c_2", "split_conv_c_3", "split_conv_c_4", "split_conv_c_5"},
    "Y", {1, dimM, 1, 1});

  AddOutput(cg, {"Y"});
}

static void createRealSplitConvs128_const(Module& pM)
{
  ComputeGraph& cg = BuildGraph(pM, "real_split_convs_128");
  AddInput(cg, "A", {1, 256, 6, 6});

  FloatTensor::ValueList bias(dimM, 0.2);
  CreateFloatWeightOperatorWithValues(cg, "C", {dimM}, bias);

  AddOperator<Split>(cg, {"A"}, {"split_conv_s_0", "split_conv_s_2"}, {{1, 128, 6, 6}, {1, 128, 6, 6}}, IntAttr(1),
                     GetInts({128, 128}));

  FloatTensor::ValueList weights(dimM * 9216, 0.9);
  for (FloatTensor::Size idx = 0; idx < weights.size(); ++idx)
    weights[idx] *= (idx + 1);

  FloatTensor::ValueList weights1, weights2;
  weights1.reserve(dimM * 128 * 6 * 6);
  weights2.reserve(dimM * 128 * 6 * 6);
  for (int a = 0; a < dimM; ++a)
    for (int b = 0; b < 256; ++b)
      for (int c = 0; c < 36; ++c) {
        if (b < 128) {
          weights1.emplace_back(weights[a * 9216 + b * 36 + c]);
        } else {
          weights2.emplace_back(weights[a * 9216 + b * 36 + c]);
        }
      }

  CreateFloatWeightOperatorWithValues(cg, "split_conv_s_1", {dimM, 128, 6, 6}, weights1);
  AddOperator<Conv>(cg, {"split_conv_s_0", "split_conv_s_1", "C"}, "split_conv_c_0", {1, dimM, 1, 1},
                    StringAttr("NOTSET"), GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}),
                    GetInts({6, 6}));

  CreateFloatWeightOperatorWithValues(cg, "split_conv_s_3", {dimM, 128, 6, 6}, weights2);
  AddOperator<Conv>(cg, {"split_conv_s_2", "split_conv_s_3"}, "split_conv_c_1", {1, dimM, 1, 1}, StringAttr("NOTSET"),
                    GetInts({1, 1}), IntAttr(1), GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));

  AddOperator<Sum>(cg, {"split_conv_c_0", "split_conv_c_1"}, "Y", {1, dimM, 1, 1});

  AddOutput(cg, {"Y"});
}

SKYPAT_F(NvDlaSplitConv, real_case_50_const)
{
  SplitConvPass::resetIndices();
  std::string ans = getNetworkString(createRealSplitConvs50_const);
  testOptPassOnNetwork<SplitConvPass>(createRealConv_const, Pass::kModuleChanged, ans,
                                      std::bind(mockGetMaxNumOfConvChannels, std::placeholders::_1, 50, 10));
}

SKYPAT_F(NvDlaSplitConv, real_case_128_const)
{
  SplitConvPass::resetIndices();
  std::string ans = getNetworkString(createRealSplitConvs128_const);
  testOptPassOnNetwork<SplitConvPass>(createRealConv_const, Pass::kModuleChanged, ans,
                                      std::bind(mockGetMaxNumOfConvChannels, std::placeholders::_1, 128, 8));
}

} // namespace onnc
