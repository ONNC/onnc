//===- EliminateIdentityTest.cpp ------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Compute/Identity.h>
#include <onnc/IR/Compute/Transpose.h>
#include <onnc/Transforms/Optimizations/EliminateIdentity.h>
#include <skypat/skypat.h>

#include "GraphUtils.h"
#include "TestUtils.h"

static void createEmptyNetwork0(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "empty_network_0");
  AddInput(cg, "input_0", {3, 1, 2});
  AddOutput(cg, {"input_0"});
}

static void createOneIdentity0(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "one_identity_0");
  AddInput(cg, "input_0", {3, 1, 2});
  AddOperator<Identity>(cg, {"input_0"}, "output_0", {3, 1, 2});
  AddOutput(cg, {"output_0"});
}

static void createOneTranspose0(Module &pM) {
  ComputeGraph &cg = BuildGraph(pM, "one_transpose_0");
  AddInput(cg, "input_0", {3, 1, 2});
  AddOperator<Transpose>(cg, {"input_0"}, "output_0", {2, 3, 1}, IntsAttr::VectorType{1, 2, 0});
  AddOutput(cg, {"output_0"});
}

//===----------------------------------------------------------------------===//
// EliminateIdentity
//===----------------------------------------------------------------------===//
SKYPAT_F(EliminateIdentity, no_identity) {
  std::string ans = getNetworkString(createOneTranspose0);
  testOptPassOnNetwork<EliminateIdentity>(createOneTranspose0,
                                          Pass::kModuleNoChanged, ans);
}

SKYPAT_F(EliminateIdentity, one_identity) {
  std::string ans = getNetworkString(createEmptyNetwork0);
  testOptPassOnNetwork<EliminateIdentity>(createOneIdentity0,
                                          Pass::kModuleChanged, ans);
}
