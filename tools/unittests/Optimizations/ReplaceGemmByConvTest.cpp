//===- ReplaceGemmByConvTest.cpp ------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Add.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Flatten.h>
#include <onnc/IR/Compute/Gemm.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Squeeze.h>
#include <onnc/IR/Compute/Unsqueeze.h>
#include <onnc/Transforms/Optimizations/ReplaceGemmByConv.h>
#include <skypat/skypat.h>

#include "GraphUtils.h"
#include "TestUtils.h"

//===----------------------------------------------------------------------===//
// One Shaping Node
//===----------------------------------------------------------------------===//

// 1 2    1 3 5    0.9 2.7 4.5
// 3 4 -> 2 4 6 -> 1.8 3.6 5.4
// 5 6

static void createBasicConv0(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "basic_conv");
  AddInput(cg, "A'", {1, 3, 1, 1});
  CreateFloatWeightOperatorWithValues(cg, "B", {2, 3, 1, 1}, {0.9, 2.7, 4.5, 1.8, 3.6, 5.4});
  CreateFloatWeightOperatorWithValues(cg, "C", {2}, {0.2, 0.4});
  AddOperator<Conv>(cg, {"A'", "B", "C"}, "replace_gemm_by_conv_0", {1, 2, 1, 1},
                    StringAttr("NOTSET"), GetInts({1, 1}), IntAttr(1),
                    GetInts({1, 1}), GetInts({0, 0, 0, 0}), GetInts({1, 1}));
  CreateWeightOperatorWithValues<Int64Tensor>(cg, "replace_gemm_by_conv_shape_0", {2}, {1, 2});
  AddOperator<Reshape>(cg, {"replace_gemm_by_conv_0", "replace_gemm_by_conv_shape_0"}, "Y", {1, 2});
  AddOutput(cg, {"Y"});
}

static void createBasicConv1(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "basic_conv");
  AddInput(cg, "A'", {1, 3, 1, 1});
  CreateFloatWeightOperatorWithValues(cg, "B", {2, 3, 1, 1}, {0.9, 2.7, 4.5, 1.8, 3.6, 5.4});
  CreateFloatWeightOperatorWithValues(cg, "C", {2}, {0.2, 0.4});
  AddOperator<Conv>(cg, {"A'", "B", "C"}, "replace_gemm_by_conv_1", {1, 2, 1, 1},
                    StringAttr("NOTSET"), GetInts({1, 1}), IntAttr(1),
                    GetInts({1, 1}), GetInts({0, 0, 0, 0}), GetInts({1, 1}));
  CreateWeightOperatorWithValues<Int64Tensor>(cg, "replace_gemm_by_conv_shape_1", {2}, {1, 2});
  AddOperator<Reshape>(cg, {"replace_gemm_by_conv_1", "replace_gemm_by_conv_shape_1"}, "Y", {1, 2});
  AddOutput(cg, {"Y"});
}

static void createBasicGemm0(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "basic_gemm_0");
  AddInput(cg, "A'", {1, 3, 1, 1});
  CreateWeightOperatorWithValues<Int64Tensor>(cg, "shape", {2}, {1, 3});
  AddOperator<Reshape>(cg, {"A'", "shape"}, "A", {1, 3});

  CreateFloatWeightOperatorWithValues(cg, "B", {3, 2}, {1, 2, 3, 4, 5, 6});
  CreateFloatWeightOperatorWithValues(cg, "C", {1, 2}, {0.1, 0.2});
  AddOperator<Gemm>(cg, {"A", "B", "C"}, "Y", {1, 2},
                    FloatAttr(0.9), FloatAttr(2.0), IntAttr(0), IntAttr(0));
  AddOutput(cg, {"Y"});
}

static void createBasicGemm1(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "basic_gemm_1");
  AddInput(cg, "A'", {1, 3, 1, 1});
  CreateWeightOperatorWithValues<Int64Tensor>(cg, "shape", {2}, {1, 3});
  AddOperator<Reshape>(cg, {"A'", "shape"}, "A", {1, 3});
  CreateFloatWeightOperatorWithValues(cg, "B", {2, 3}, {1, 3, 5, 2, 4, 6});
  CreateFloatWeightOperatorWithValues(cg, "C", {2}, {0.1, 0.2});
  AddOperator<Gemm>(cg, {"A", "B", "C"}, "Y", {1, 2},
                    FloatAttr(0.9), FloatAttr(2.0), IntAttr(0), IntAttr(1));
  AddOutput(cg, {"Y"});
}

SKYPAT_F(ReplaceGemmByConv, basic_gemm_0) {
  std::string ans = getNetworkString(createBasicConv0);
  testOptPassOnNetwork<ReplaceGemmByConv>(createBasicGemm0,
                                          Pass::kModuleChanged, ans);
}

SKYPAT_F(ReplaceGemmByConv, basic_gemm_1) {
  std::string ans = getNetworkString(createBasicConv1);
  testOptPassOnNetwork<ReplaceGemmByConv>(createBasicGemm1,
                                          Pass::kModuleChanged, ans);
}

//===----------------------------------------------------------------------===//
// More Shaping Nodes
//===----------------------------------------------------------------------===//
static void createComplexConv(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "complex_conv");
  AddInput(cg, "A'", {1, 3, 2, 2});

  FloatTensor::ValueList vals(24, 0.9);
  CreateFloatWeightOperatorWithValues(cg, "B", {2, 3, 2, 2}, vals);
  CreateFloatWeightOperatorWithValues(cg, "C", {2}, {0.2, 0.4});

  AddOperator<Conv>(cg, {"A'", "B", "C"}, "replace_gemm_by_conv_2", {1, 2, 1, 1},
                    StringAttr("NOTSET"), GetInts({1, 1}), IntAttr(1),
                    GetInts({2, 2}), GetInts({0, 0, 0, 0}), GetInts({1, 1}));
  CreateWeightOperatorWithValues<Int64Tensor>(cg, "replace_gemm_by_conv_shape_2", {2}, {1, 2});
  AddOperator<Reshape>(cg, {"replace_gemm_by_conv_2", "replace_gemm_by_conv_shape_2"}, "Y", {1, 2});
  AddOutput(cg, {"Y"});
}

static void createGemmWithShapingNodes(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "basic_gemm_0");
  AddInput(cg, "A'", {1, 3, 2, 2});
  AddOperator<Squeeze>(cg, {"A'"}, "s0", {3, 2, 2}, GetInts({0}));
  AddOperator<Flatten>(cg, {"s0"}, "s1", {3, 4}, IntAttr(1));
  AddOperator<Unsqueeze>(cg, {"s1"}, "s2", {3, 1, 4}, GetInts({1}));

  CreateWeightOperator<Int64Tensor>(cg, "shape", {1, 12});
  AddOperator<Reshape>(cg, {"s2", "shape"}, "A", {1, 12});

  FloatTensor::ValueList vals(24, 1.0);
  CreateFloatWeightOperatorWithValues(cg, "B", {12, 2}, vals);
  CreateFloatWeightOperatorWithValues(cg, "C", {1, 2}, {0.1, 0.2});

  AddOperator<Gemm>(cg, {"A", "B", "C"}, "Y", {1, 2},
                    FloatAttr(0.9), FloatAttr(2.0), IntAttr(0), IntAttr(0));
  AddOutput(cg, {"Y"});
}

SKYPAT_F(ReplaceGemmByConv, gemm_with_shaping_nodes) {
  std::string ans = getNetworkString(createComplexConv);
  testOptPassOnNetwork<ReplaceGemmByConv>(createGemmWithShapingNodes,
                                          Pass::kModuleChanged, ans);
}

//===----------------------------------------------------------------------===//
// Real Case
//===----------------------------------------------------------------------===//
static const int dimM = 2;
static void createRealConv(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "real_conv");
  AddInput(cg, "A'", {1, 256, 6, 6});

  FloatTensor::ValueList weights(9216 * dimM, 0.9), bias(dimM, 0.2);
  CreateFloatWeightOperatorWithValues(cg, "B", {dimM, 256, 6, 6}, weights);
  CreateFloatWeightOperatorWithValues(cg, "C", {dimM}, bias);

  AddOperator<Conv>(cg, {"A'", "B", "C"}, "replace_gemm_by_conv_3", {1, dimM, 1, 1},
                    StringAttr("NOTSET"), GetInts({1, 1}), IntAttr(1),
                    GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({1, 1}));
  CreateWeightOperatorWithValues<Int64Tensor>(cg, "replace_gemm_by_conv_shape_3", {2}, {1, dimM});
  AddOperator<Reshape>(cg, {"replace_gemm_by_conv_3", "replace_gemm_by_conv_shape_3"}, "Y", {1, dimM});
  AddOutput(cg, {"Y"});
}

static void createRealGemm(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "real_gemm");
  AddInput(cg, "A'", {1, 256, 6, 6});
  CreateWeightOperator<Int64Tensor>(cg, "shape", {1, 9216});
  AddOperator<Reshape>(cg, {"A'", "shape"}, "A", {1, 9216});

  FloatTensor::ValueList weights(9216 * dimM, 1.0), bias(dimM, 0.1);
  CreateFloatWeightOperatorWithValues(cg, "B", {9216, dimM}, weights);
  CreateFloatWeightOperatorWithValues(cg, "C", {1, dimM}, bias);

  AddOperator<Gemm>(cg, {"A", "B", "C"}, "Y", {1, dimM},
                    FloatAttr(0.9), FloatAttr(2.0), IntAttr(0), IntAttr(0));
  AddOutput(cg, {"Y"});
}

// This one needs a lot of time to run if set dimM = 4096 ...
SKYPAT_F(ReplaceGemmByConv, real_case) {
  std::string ans = getNetworkString(createRealConv);
  testOptPassOnNetwork<ReplaceGemmByConv>(createRealGemm,
                                          Pass::kModuleChanged, ans);
}

//===----------------------------------------------------------------------===//
// Zero Shaping Node
//===----------------------------------------------------------------------===//

static void createBasicConv2(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "basic_conv_2");
  AddInput(cg, "A", {1, 3});
  CreateFloatWeightOperatorWithValues(cg, "B", {2, 3, 1, 1}, {0.9, 2.7, 4.5, 1.8, 3.6, 5.4});
  CreateFloatWeightOperatorWithValues(cg, "C", {2}, {0.2, 0.4});
  CreateWeightOperatorWithValues<Int64Tensor>(cg, "replace_gemm_by_conv_shape_4", {4}, {1, 3, 1, 1});
  AddOperator<Reshape>(cg, {"A", "replace_gemm_by_conv_shape_4"}, "replace_gemm_by_conv_reshape_0", {1, 3, 1, 1});
  AddOperator<Conv>(cg, {"replace_gemm_by_conv_reshape_0", "B", "C"}, "replace_gemm_by_conv_4", {1, 2, 1, 1},
                    StringAttr("NOTSET"), GetInts({1, 1}), IntAttr(1),
                    GetInts({1, 1}), GetInts({0, 0, 0, 0}), GetInts({1, 1}));
  CreateWeightOperatorWithValues<Int64Tensor>(cg, "replace_gemm_by_conv_shape_5", {2}, {1, 2});
  AddOperator<Reshape>(cg, {"replace_gemm_by_conv_4", "replace_gemm_by_conv_shape_5"}, "Y", {1, 2});
  AddOutput(cg, {"Y"});
}

static void createBasicGemm2(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "basic_gemm_2");
  AddInput(cg, "A", {1, 3});
  CreateFloatWeightOperatorWithValues(cg, "B", {2, 3}, {1, 3, 5, 2, 4, 6});
  CreateFloatWeightOperatorWithValues(cg, "C", {2}, {0.1, 0.2});
  AddOperator<Gemm>(cg, {"A", "B", "C"}, "Y", {1, 2},
                    FloatAttr(0.9), FloatAttr(2.0), IntAttr(0), IntAttr(1));
  AddOutput(cg, {"Y"});
}

SKYPAT_F(ReplaceGemmByConv, basic_gemm_2) {
  std::string ans = getNetworkString(createBasicConv2);
  testOptPassOnNetwork<ReplaceGemmByConv>(createBasicGemm2,
                                          Pass::kModuleChanged, ans);
}
