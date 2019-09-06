//===- DivideGlobalAPIntoAPsTest.cpp --------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/AveragePool.h>
#include <onnc/IR/Compute/GlobalAveragePool.h>
#include <onnc/IR/Compute/Mul.h>
#include <onnc/Transforms/Optimizations/DivideGlobalAPIntoAPs.h>
#include <skypat/skypat.h>

#include "GraphUtils.h"
#include "TestUtils.h"

static void createAP223(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "ap_223");
  AddInput(cg, "input_0", {1, 1000, 2, 2, 3});
  AddOperator<AveragePool>(cg, {"input_0"}, "output_0", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(0),
                           GetInts({2, 2, 3}), GetInts({0, 0, 0, 0, 0, 0}), GetInts({2, 2, 3}));
  AddOutput(cg, {"output_0"});
}

static void createAP8(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "ap_8");
  AddInput(cg, "input_0", {1, 1000, 8, 8});
  AddOperator<AveragePool>(cg, {"input_0"}, "output_0", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(0),
                           GetInts({8, 8}), GetInts({0, 0, 0, 0}), GetInts({8, 8}));
  AddOutput(cg, {"output_0"});
}

// AveragePool(9x9): AveragePool(5x5) -> AveragePool(2x2) -> Mul(100/81)
// static void createAPsMul1(Module &pM) {
  // ComputeGraph &cg = BuildGraph(pM, "aps_mul_1");
  // AddInput(cg, "input_0", {1, 1000, 9, 9});
  // AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_0", {1, 1000, 2, 2},
                            // StringAttr("NOTSET"), IntAttr(1),
                            // GetInts({5, 5}), GetInts({1, 1, 0, 0}), GetInts({5, 5}));
  // AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_0"},
                                // "divide_globalap_into_aps_1", {1, 1000, 1, 1},
                            // StringAttr("NOTSET"), IntAttr(1),
                            // GetInts({2, 2}), GetInts({0, 0, 0, 0}), GetInts({2, 2}));
  // CreateFloatWeightOperatorWithValues(cg, "divide_globalap_into_aps_2", {1}, {100.0 / 81.0});
  // AddOperator<Mul>(cg, {"divide_globalap_into_aps_1", "divide_globalap_into_aps_2"},
                       // "output_0", {1, 1000, 1, 1});
  // AddOutput(cg, {"output_0"});
// }

// AveragePool(9x9): AveragePool(3x3) -> AveragePool(3x3)
static void createAPsMul33(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_33");
  AddInput(cg, "input_0", {1, 1000, 9, 9});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_0", {1, 1000, 3, 3},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({3, 3}), GetInts({0, 0, 0, 0}), GetInts({3, 3}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_0"}, "output_0", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({3, 3}), GetInts({0, 0, 0, 0}), GetInts({3, 3}));
  AddOutput(cg, {"output_0"});
}

// AveragePool(10x10): AveragePool(5x5) -> AveragePool(2x2)
static void createAPsMul52(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_52");
  AddInput(cg, "input_0", {1, 1000, 10, 10});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_1", {1, 1000, 2, 2},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({5, 5}), GetInts({0, 0, 0, 0}), GetInts({5, 5}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_1"}, "output_0", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({2, 2}), GetInts({0, 0, 0, 0}), GetInts({2, 2}));
  AddOutput(cg, {"output_0"});
}

// AveragePool(11x11): AveragePool(6x6) -> AveragePool(2x2) -> Mul(144/121)
static void createAPsMul62_1(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_62_1");
  AddInput(cg, "input_0", {1, 1000, 11, 11});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_2", {1, 1000, 2, 2},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({6, 6}), GetInts({1, 1, 0, 0}), GetInts({6, 6}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_2"}, 
                                "divide_globalap_into_aps_3", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({2, 2}), GetInts({0, 0, 0, 0}), GetInts({2, 2}));
  FloatTensor::ValueList values(1, 144.0 / 121.0);
  CreateFloatWeightOperatorWithValues(cg, "divide_globalap_into_aps_4", {1}, values);
  AddOperator<Mul>(cg, {"divide_globalap_into_aps_3", "divide_globalap_into_aps_4"},
                       "output_0", {1, 1000, 1, 1});
  AddOutput(cg, {"output_0"});
}

// AveragePool(12x12): AveragePool(6x6) -> AveragePool(2x2)
static void createAPsMul62(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_62");
  AddInput(cg, "input_0", {1, 1000, 12, 12});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_5", {1, 1000, 2, 2},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_5"}, 
                                "output_0", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({2, 2}), GetInts({0, 0, 0, 0}), GetInts({2, 2}));
  AddOutput(cg, {"output_0"});
}

// AveragePool(13x13): AveragePool(7x7) -> AveragePool(2x2) -> Mul(196/169)
static void createAPsMul72_1(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_72_1");
  AddInput(cg, "input_0", {1, 1000, 13, 13});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_6", {1, 1000, 2, 2},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({7, 7}), GetInts({1, 1, 0, 0}), GetInts({7, 7}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_6"},
                                "divide_globalap_into_aps_7", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({2, 2}), GetInts({0, 0, 0, 0}), GetInts({2, 2}));
  FloatTensor::ValueList values(1, 196.0 / 169.0);
  CreateFloatWeightOperatorWithValues(cg, "divide_globalap_into_aps_8", {1}, values);
  AddOperator<Mul>(cg, {"divide_globalap_into_aps_7", "divide_globalap_into_aps_8"},
                       "output_0", {1, 1000, 1, 1});
  AddOutput(cg, {"output_0"});
}

// AveragePool(14x14): AveragePool(7x7) -> AveragePool(2x2)
static void createAPsMul72(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_72");
  AddInput(cg, "input_0", {1, 1000, 14, 14});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_9", {1, 1000, 2, 2},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({7, 7}), GetInts({0, 0, 0, 0}), GetInts({7, 7}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_9"},
                                "output_0", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({2, 2}), GetInts({0, 0, 0, 0}), GetInts({2, 2}));
  AddOutput(cg, {"output_0"});
}

// AveragePool(27x27): AveragePool(7x7) -> AveragePool(4x4) -> Mul(784/729)
static void createAPsMul74_1(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_74_1");
  AddInput(cg, "input_0", {1, 1000, 27, 27});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_10", {1, 1000, 4, 4},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({7, 7}), GetInts({1, 1, 0, 0}), GetInts({7, 7}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_10"},
                                "divide_globalap_into_aps_11", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({4, 4}), GetInts({0, 0, 0, 0}), GetInts({4, 4}));
  FloatTensor::ValueList values(1, 784.0 / 729.0);
  CreateFloatWeightOperatorWithValues(cg, "divide_globalap_into_aps_12", {1}, values);
  AddOperator<Mul>(cg, {"divide_globalap_into_aps_11", "divide_globalap_into_aps_12"},
                       "output_0", {1, 1000, 1, 1});
  AddOutput(cg, {"output_0"});
}

// AveragePool(150x150): AveragePool(6x6) -> AveragePool(5x5) -> AveragePool(5x5)
static void createAPsMul655_0(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_655_0");
  AddInput(cg, "input_0", {1, 1000, 150, 150});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_13", {1, 1000, 25, 25},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({6, 6}), GetInts({0, 0, 0, 0}), GetInts({6, 6}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_13"},
                                "divide_globalap_into_aps_14", {1, 1000, 5, 5},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({5, 5}), GetInts({0, 0, 0, 0}), GetInts({5, 5}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_14"},
                                "output_0", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({5, 5}), GetInts({0, 0, 0, 0}), GetInts({5, 5}));
  AddOutput(cg, {"output_0"});
}

// AveragePool(151x151): AveragePool(8x8) -> AveragePool(5x5) -> AveragePool(4x4) -> Mul(160^2/151^2)
static void createAPsMul854_9(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_854_9");
  AddInput(cg, "input_0", {1, 1000, 151, 151});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_15", {1, 1000, 19, 19},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({8, 8}), GetInts({1, 1, 0, 0}), GetInts({8, 8}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_15"},
                                "divide_globalap_into_aps_16", {1, 1000, 4, 4},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({5, 5}), GetInts({1, 1, 0, 0}), GetInts({5, 5}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_16"},
                                "divide_globalap_into_aps_17", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({4, 4}), GetInts({0, 0, 0, 0}), GetInts({4, 4}));
  FloatTensor::ValueList values(1, 160.0 * 160.0 / 151.0 / 151.0);
  CreateFloatWeightOperatorWithValues(cg, "divide_globalap_into_aps_18", {1}, values);
  AddOperator<Mul>(cg, {"divide_globalap_into_aps_17", "divide_globalap_into_aps_18"},
                   "output_0", {1, 1000, 1, 1});
  AddOutput(cg, {"output_0"});
}

static void createOneGlobalAP0(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "one_globalap_0");
  AddInput(cg, "input_0", {1, 1000, 2, 2, 3});
  AddOperator<GlobalAveragePool>(cg, {"input_0"}, "output_0", {1, 1000, 1, 1});
  AddOutput(cg, {"output_0"});
}

static void createOneGlobalAP1(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "one_globalap_1");
  AddInput(cg, "input_0", {1, 1000, 7, 7, 7});
  AddOperator<GlobalAveragePool>(cg, {"input_0"}, "output_0", {1, 1000, 1, 1});
  AddOutput(cg, {"output_0"});
}

static void createOneGlobalAP2(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "one_globalap_2");
  AddInput(cg, "input_0", {1, 1000, 9, 10});
  AddOperator<GlobalAveragePool>(cg, {"input_0"}, "output_0", {1, 1000, 1, 1});
  AddOutput(cg, {"output_0"});
}

static void createOneGlobalAP8(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "one_globalap_8");
  AddInput(cg, "input_0", {1, 1000, 8, 8});
  AddOperator<GlobalAveragePool>(cg, {"input_0"}, "output_0", {1, 1000, 1, 1});
  AddOutput(cg, {"output_0"});
}

// a function to create one GlobalAveragePool with variable size
struct createOneGlobalAPwithSize {
  createOneGlobalAPwithSize(unsigned sz): createOneGlobalAPwithSize(sz, sz) {}
  createOneGlobalAPwithSize(unsigned H, unsigned W): hSize(H), wSize(W) {}
  void operator() (Module &pM) {
    ComputeGraph &cg = BuildGraph(pM, "one_globalap_" +
                                      std::to_string(hSize) + std::to_string(wSize));
    AddInput(cg, "input_0", {1, 1000, hSize, wSize});
    AddOperator<GlobalAveragePool>(cg, {"input_0"}, "output_0", {1, 1000, 1, 1});
    AddOutput(cg, {"output_0"});
  }
  unsigned hSize, wSize;
};

//===----------------------------------------------------------------------===//
// DivideGlobalAPIntoAPs
//===----------------------------------------------------------------------===//
// // This one will cause assertion failed under current restriction on dims
// SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_223_0) {
//   std::string ans = getNetworkString(createAP223);
//   testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAP0,
//                                               Pass::kModuleChanged, ans);
// }

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_8) {
  std::string ans = getNetworkString(createAP8);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(8),
                                              Pass::kModuleChanged, ans);
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_9) {
  std::string ans = getNetworkString(createAPsMul33);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(9),
                                              Pass::kModuleChanged, ans);
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_10) {
  std::string ans = getNetworkString(createAPsMul52);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(10),
                                              Pass::kModuleChanged, ans);
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_11) {
  std::string ans = getNetworkString(createAPsMul62_1);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(11),
                                              Pass::kModuleChanged, ans);
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_12) {
  std::string ans = getNetworkString(createAPsMul62);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(12),
                                              Pass::kModuleChanged, ans);
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_13) {
  std::string ans = getNetworkString(createAPsMul72_1);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(13),
                                              Pass::kModuleChanged, ans);
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_14) {
  std::string ans = getNetworkString(createAPsMul72);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(14),
                                              Pass::kModuleChanged, ans);
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_27) {
  std::string ans = getNetworkString(createAPsMul74_1);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(27),
                                              Pass::kModuleChanged, ans);
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_150) {
  std::string ans = getNetworkString(createAPsMul655_0);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(150),
                                              Pass::kModuleChanged, ans);
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_151) {
  std::string ans = getNetworkString(createAPsMul854_9);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(151),
                                              Pass::kModuleChanged, ans);
}

//===----------------------------------------------------------------------===//
// DivideGlobalAPIntoAPs - Not in square
//===----------------------------------------------------------------------===//
// AveragePool(9x8): AveragePool(3x3) -> AveragePool(3x3)
static void createAPsMul_9x8(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_9x8");
  AddInput(cg, "input_0", {1, 1000, 9, 8});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_19", {1, 1000, 3, 3},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({3, 3}), GetInts({0, 1, 0, 0}), GetInts({3, 3}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_19"}, "divide_globalap_into_aps_20", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({3, 3}), GetInts({0, 0, 0, 0}), GetInts({3, 3}));
  FloatTensor::ValueList values(1, 81.0 / 72.0);
  CreateFloatWeightOperatorWithValues(cg, "divide_globalap_into_aps_21", {1}, values);
  AddOperator<Mul>(cg, {"divide_globalap_into_aps_20", "divide_globalap_into_aps_21"},
                   "output_0", {1, 1000, 1, 1});
  AddOutput(cg, {"output_0"});
}

// AveragePool(8x9): AveragePool(3x3) -> AveragePool(3x3)
static void createAPsMul_8x9(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_8x9");
  AddInput(cg, "input_0", {1, 1000, 8, 9});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_22", {1, 1000, 3, 3},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({3, 3}), GetInts({1, 0, 0, 0}), GetInts({3, 3}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_22"}, "divide_globalap_into_aps_23", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({3, 3}), GetInts({0, 0, 0, 0}), GetInts({3, 3}));
  FloatTensor::ValueList values(1, 81.0 / 72.0);
  CreateFloatWeightOperatorWithValues(cg, "divide_globalap_into_aps_24", {1}, values);
  AddOperator<Mul>(cg, {"divide_globalap_into_aps_23", "divide_globalap_into_aps_24"},
                   "output_0", {1, 1000, 1, 1});
  AddOutput(cg, {"output_0"});
}

// AveragePool(9x5): AveragePool(3x3) -> AveragePool(3x2)
static void createAPsMul_9x5(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_9x5");
  AddInput(cg, "input_0", {1, 1000, 9, 5});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_25", {1, 1000, 3, 2},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({3, 3}), GetInts({0, 1, 0, 0}), GetInts({3, 3}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_25"}, "divide_globalap_into_aps_26", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({3, 2}), GetInts({0, 0, 0, 0}), GetInts({3, 2}));
  FloatTensor::ValueList values(1, 54.0 / 45.0);
  CreateFloatWeightOperatorWithValues(cg, "divide_globalap_into_aps_27", {1}, values);
  AddOperator<Mul>(cg, {"divide_globalap_into_aps_26", "divide_globalap_into_aps_27"},
                   "output_0", {1, 1000, 1, 1});
  AddOutput(cg, {"output_0"});
}

// AveragePool(9x2): AveragePool(3x2) -> AveragePool(3x1)
static void createAPsMul_9x2(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_9x2");
  AddInput(cg, "input_0", {1, 1000, 9, 2});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_28", {1, 1000, 3, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({3, 2}), GetInts({0, 0, 0, 0}), GetInts({3, 2}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_28"}, "output_0", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({3, 1}), GetInts({0, 0, 0, 0}), GetInts({3, 1}));
  AddOutput(cg, {"output_0"});
}

// AveragePool(9x1): AveragePool(3x1) -> AveragePool(3x1)
static void createAPsMul_9x1(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_9x1");
  AddInput(cg, "input_0", {1, 1000, 9, 1});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_29", {1, 1000, 3, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({3, 1}), GetInts({0, 0, 0, 0}), GetInts({3, 1}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_29"}, "output_0", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({3, 1}), GetInts({0, 0, 0, 0}), GetInts({3, 1}));
  AddOutput(cg, {"output_0"});
}

// AveragePool(67x151): AveragePool(8x8) -> AveragePool(5x5) -> AveragePool(2,4)
static void createAPsMul_67x151(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "aps_mul_67x151");
  AddInput(cg, "input_0", {1, 1000, 67, 151});
  AddOperator<AveragePool>(cg, {"input_0"}, "divide_globalap_into_aps_30", {1, 1000, 9, 19},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({8, 8}), GetInts({5, 1, 0, 0}), GetInts({8, 8}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_30"}, "divide_globalap_into_aps_31", {1, 1000, 2, 4},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({5, 5}), GetInts({1, 1, 0, 0}), GetInts({5, 5}));
  AddOperator<AveragePool>(cg, {"divide_globalap_into_aps_31"}, "divide_globalap_into_aps_32", {1, 1000, 1, 1},
                           StringAttr("NOTSET"), IntAttr(1),
                           GetInts({2, 4}), GetInts({0, 0, 0, 0}), GetInts({2, 4}));
  FloatTensor::ValueList values(1, (160.0 * 80.0) / (67.0 * 151.0));
  CreateFloatWeightOperatorWithValues(cg, "divide_globalap_into_aps_33", {1}, values);
  AddOperator<Mul>(cg, {"divide_globalap_into_aps_32", "divide_globalap_into_aps_33"},
                   "output_0", {1, 1000, 1, 1});
  AddOutput(cg, {"output_0"});
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_9x8) {
  std::string ans = getNetworkString(createAPsMul_9x8);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(9, 8),
                                              Pass::kModuleChanged, ans);
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_8x9) {
  std::string ans = getNetworkString(createAPsMul_8x9);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(8, 9),
                                              Pass::kModuleChanged, ans);
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_9x5) {
  std::string ans = getNetworkString(createAPsMul_9x5);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(9, 5),
                                              Pass::kModuleChanged, ans);
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_9x2) {
  std::string ans = getNetworkString(createAPsMul_9x2);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(9, 2),
                                              Pass::kModuleChanged, ans);
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_9x1) {
  std::string ans = getNetworkString(createAPsMul_9x1);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(9, 1),
                                              Pass::kModuleChanged, ans);
}

SKYPAT_F(DivideGlobalAPIntoAPs, one_globalap_67x151) {
  std::string ans = getNetworkString(createAPsMul_67x151);
  testOptPassOnNetwork<DivideGlobalAPIntoAPs>(createOneGlobalAPwithSize(67, 151),
                                              Pass::kModuleChanged, ans);
}
