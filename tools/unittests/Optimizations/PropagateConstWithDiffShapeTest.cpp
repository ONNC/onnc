//===- PropagateConstWithDiffShapeTest.cpp --------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Add.h>
#include <onnc/IR/Compute/Flatten.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Squeeze.h>
#include <onnc/IR/Compute/Unsqueeze.h>
#include <onnc/Transforms/Optimizations/PropagateConstWithDiffShape.h>
#include <skypat/skypat.h>

#include "GraphUtils.h"
#include "TestUtils.h"

static void createOneAddInitial0(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "one_add_initial_0");
  AddInput(cg, "input_0", {5, 5, 3});
  CreateFloatWeightOperator(cg, "initial_0", {5, 5, 3});
  AddOperator<Add>(cg, {"input_0", "initial_0"}, "output_0", {5, 5, 3});
  AddOutput(cg, {"output_0"});
}

static void createConstSqueeze0(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "const_squeeze_0");
  AddInput(cg, "input_0", {5, 5, 3});
  CreateFloatWeightOperator(cg, "initial_0", {1, 5, 1, 5, 3});
  AddOperator<Squeeze>(cg, {"initial_0"}, "squeeze_0", {5, 5, 3}, GetInts({0, 2}));
  AddOperator<Add>(cg, {"input_0", "squeeze_0"}, "output_0", {5, 5, 3});
  AddOutput(cg, {"output_0"});
}

static void createConstSqueeze1(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "const_squeeze_1");
  AddInput(cg, "input_0", {5, 5, 3});
  CreateFloatWeightOperator(cg, "initial_0", {1, 5, 1, 5, 3});
  AddOperator<Squeeze>(cg, {"initial_0"}, "squeeze_0", {1, 5, 5, 3}, GetInts({2}));
  AddOperator<Squeeze>(cg, {"squeeze_0"}, "squeeze_1", {5, 5, 3}, GetInts({0}));
  AddOperator<Add>(cg, {"input_0", "squeeze_1"}, "output_0", {5, 5, 3});
  AddOutput(cg, {"output_0"});
}

static void createConstShaping0(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "const_shaping_0");
  AddInput(cg, "input_0", {5, 5, 3});
  CreateFloatWeightOperator(cg, "initial_0", {1, 5, 1, 5, 3});

  AddOperator<Squeeze>(cg, {"initial_0"}, "squeeze_0", {5, 5, 3}, GetInts({0, 2}));
  AddOperator<Unsqueeze>(cg, {"squeeze_0"}, "unsqueeze_0", {5, 1, 5, 1, 3, 1}, GetInts({1, 3, 5}));
  AddOperator<Flatten>(cg, {"unsqueeze_0"}, "flatten_0", {25, 3}, IntAttr({3}));

  CreateWeightOperatorWithValues<Int64Tensor>(cg, "shape", {5}, {1, 5, 5, 3, 1});
  AddOperator<Reshape>(cg, {"flatten_0", "shape"}, "reshape_0", {1, 5, 5, 3, 1});

  AddOperator<Squeeze>(cg, {"reshape_0"}, "squeeze_1", {5, 5, 3}, GetInts({0, 4}));

  AddOperator<Add>(cg, {"input_0", "squeeze_1"}, "output_0", {5, 5, 3});
  AddOutput(cg, {"output_0"});
}

//===----------------------------------------------------------------------===//
// PropagateConstWithDiffShape
//===----------------------------------------------------------------------===//
SKYPAT_F(PropagateConstWithDiffShape, one_const_squeeze) {
  std::string ans = getNetworkString(createOneAddInitial0);
  testOptPassOnNetwork<PropagateConstWithDiffShape>(createConstSqueeze0,
                                                    Pass::kModuleChanged, ans);
}

SKYPAT_F(PropagateConstWithDiffShape, one_const_two_squeezes) {
  std::string ans = getNetworkString(createOneAddInitial0);
  testOptPassOnNetwork<PropagateConstWithDiffShape>(createConstSqueeze1,
                                                    Pass::kModuleChanged, ans);
}

SKYPAT_F(PropagateConstWithDiffShape, one_const_shaping_nodes_0) {
  std::string ans = getNetworkString(createOneAddInitial0);
  testOptPassOnNetwork<PropagateConstWithDiffShape>(createConstShaping0,
                                                    Pass::kModuleChanged, ans);
}

//===----------------------------------------------------------------------===//
// Initializer is used by two nodes
//===----------------------------------------------------------------------===//
static void createOneAddInitial1(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "one_add_initial_1");
  AddInput(cg, "input_0", {5, 5, 3});
  FloatTensor::ValueList values(25, 1.0);
  CreateFloatWeightOperatorWithValues(cg, "initial_0", {1, 5, 1, 5, 3}, values);
  AddOutput(cg, {"initial_0"});

  CreateFloatWeightOperatorWithValues(cg, "initial_0<clone>", {5, 5, 3}, values);
  AddOperator<Add>(cg, {"input_0", "initial_0<clone>"}, "output_0", {5, 5, 3});
  AddOutput(cg, {"output_0"});
}

static void createConstShaping1(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "const_shaping_1");
  AddInput(cg, "input_0", {5, 5, 3});
  FloatTensor::ValueList values(25, 1.0);
  CreateFloatWeightOperatorWithValues(cg, "initial_0", {1, 5, 1, 5, 3}, values);

  CreateWeightOperatorWithValues<Int64Tensor>(cg, "shape", {5}, {1, 5, 5, 3, 1});

  AddOperator<Reshape>(cg, {"initial_0", "shape"}, "reshape_0", {1, 5, 5, 3, 1});

  AddOperator<Squeeze>(cg, {"reshape_0"}, "squeeze_0", {5, 5, 3}, GetInts({0, 4}));

  AddOperator<Add>(cg, {"input_0", "squeeze_0"}, "output_0", {5, 5, 3});
  AddOutput(cg, {"output_0"});
  AddOutput(cg, {"initial_0"});
}

SKYPAT_F(PropagateConstWithDiffShape, clone_initializer) {
  std::string ans = getNetworkString(createOneAddInitial1);
  testOptPassOnNetwork<PropagateConstWithDiffShape>(createConstShaping1,
                                                    Pass::kModuleChanged, ans);
}
