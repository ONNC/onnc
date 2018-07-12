//===- TensorSelTest.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/Transforms/PreTensorSel.h>
#include <onnc/Transforms/TensorSel.h>
#include <onnc/IR/IRBuilder.h>
#include <onnc/Core/PassManager.h>
#include <onnc/Transforms/removeUnusedNodes.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Any Test
//===----------------------------------------------------------------------===//
SKYPAT_F(TensorSelTest, select_stanard_graph)
{
  onnc::Module module;
  IRBuilder builder(module);

  builder.CreateTensorGraph();
  builder.AddInput("data_0", {10, 3, 277, 277});
  builder.AddInput("conv1_w_0", {96, 3, 11, 11});
  builder.AddInput("conv1_b_0", {96});
  builder.AddInput("conv2_w_0", {256, 48, 5, 5});
  builder.AddInput("conv2_b_0", {256});
  builder.AddInput("conv3_w_0", {384, 256, 3, 3});
  builder.AddInput("conv3_b_0", {384});
  builder.AddInput("conv4_w_0", {384, 192, 3, 3});
  builder.AddInput("conv4_b_0", {384});
  builder.AddInput("conv5_w_0", {256, 192, 3, 3});
  builder.AddInput("conv5_b_0", {256});
  builder.AddInput("fc6_w_0", {4096, 9216});
  builder.AddInput("fc6_b_0", {4096});
  builder.AddInput("fc7_w_0", {4096, 4096});
  builder.AddInput("fc7_b_0", {4096});
  builder.AddInput("fc8_w_0", {1000, 4096});
  builder.AddInput("fc8_b_0", {1000});

  builder.AddInitializer("conv1_w_0");
  builder.AddInitializer("conv1_b_0");
  builder.AddInitializer("conv2_w_0");
  builder.AddInitializer("conv2_b_0");
  builder.AddInitializer("conv3_w_0");
  builder.AddInitializer("conv3_b_0");
  builder.AddInitializer("conv4_w_0");
  builder.AddInitializer("conv4_b_0");
  builder.AddInitializer("conv5_w_0");
  builder.AddInitializer("conv5_b_0");
  builder.AddInitializer("fc6_w_0");
  builder.AddInitializer("fc6_b_0");
  builder.AddInitializer("fc7_w_0");
  builder.AddInitializer("fc7_b_0");
  builder.AddInitializer("fc8_w_0");
  builder.AddInitializer("fc8_b_0");

  // create nodes (layers)
  ::onnx::Node* conv = builder.AddNode("Conv",    {"data_0", "conv1_w_0", "conv1_b_0"});
  builder.AddOutput("conv1_1", {1});

  builder.AddNode("Relu",    {"conv1_1"});
  builder.AddOutput("conv1_2", {1});

  builder.AddNode("LRN",     {"conv1_2"});
  builder.AddOutput("norm1_1", {1});

  builder.AddNode("MaxPool", {"norm1_1"});
  builder.AddOutput("pool1_1", {1});

  builder.AddNode("Conv",    {"pool1_1", "conv2_w_0", "conv2_b_0"});
  builder.AddOutput("conv2_1", {1});

  builder.AddNode("Relu",    {"conv2_1"});
  builder.AddOutput("conv2_2", {1});

  builder.AddNode("LRN",     {"conv2_2"});
  builder.AddOutput("norm2_1", {1});

  builder.AddNode("MaxPool", {"norm2_1"});
  builder.AddOutput("pool2_1", {1});

  builder.AddNode("Conv",    {"pool2_1", "conv3_w_0", "conv3_b_0"});     
  builder.AddOutput("conv3_1", {1});

  builder.AddNode("Relu",    {"conv3_1"});
  builder.AddOutput("conv3_2", {1});

  builder.AddNode("Conv",    {"conv3_2", "conv4_w_0", "conv4_b_0"});     
  builder.AddOutput("conv4_1", {1});

  builder.AddNode("Relu",    {"conv4_1"});
  builder.AddOutput("conv4_2", {1});

  builder.AddNode("Conv",    {"conv4_2", "conv5_w_0", "conv5_b_0"});
  builder.AddOutput("conv5_1", {1});

  builder.AddNode("Relu",    {"conv5_1"});
  builder.AddOutput("conv5_2", {1});

  builder.AddNode("MaxPool", {"conv5_2"});
  builder.AddOutput("pool5_1", {1});

  builder.AddNode("Gemm",    {"pool5_1", "fc6_w_0", "fc6_b_0"});     
  builder.AddOutput("fc6_1", {1});

  builder.AddNode("Relu",    {"fc6_1"});
  builder.AddOutput("fc6_2", {1});

  builder.AddNode("Dropout", {"fc6_2"});
  builder.AddOutput("fc6_3", {1});                                   
  builder.AddOutput("_fc6_mask_1", {1});

  builder.AddNode("Gemm",    {"fc6_3", "fc7_w_0", "fc7_b_0"});
  builder.AddOutput("fc7_1", {1});

  builder.AddNode("Relu",    {"fc7_1"});
  builder.AddOutput("fc7_2", {1});

  builder.AddNode("Dropout", {"fc7_2"});
  builder.AddOutput("fc7_3", {1});                                   
  builder.AddOutput("_fc7_mask_1", {1});

  builder.AddNode("Gemm",    {"fc7_3", "fc8_w_0", "fc8_b_0"});
  builder.AddOutput("fc8_1", {1});

  builder.AddNode("Softmax", {"fc8_1"});
  builder.AddOutput("prob_1", {1});

  builder.FinalizeTensorGraph({"prob_1"});

  PassRegistry registry;
  PassManager pm(registry);
  PassManager::State state;

  pm.add(createRemoveUnusedNodesPass(), state);
  pm.add(CreateUpdateGraphOutputSizePass(), state);
  pm.add(CreatePreTensorSel(), state);

  /// create tensor selection
  TensorSel* tensor_selection = new TensorSel();
  pm.add(tensor_selection, state);

  // RemoveUnusedNodesPass
  pm.step(module, state);
  errs() << state.pass->getPassName() << std::endl;

  // UpdateGraphOutputSizePass
  pm.step(module, state);
  errs() << state.pass->getPassName() << std::endl;

  // PreTensorSel
  pm.step(module, state);
  errs() << state.pass->getPassName() << std::endl;

  tensor_selection->getLowerRegistry().add(ConvLower);
  // TensorSel
  pm.step(module, state);
  errs() << state.pass->getPassName() << std::endl;

  ASSERT_TRUE(state.execution.empty());
  ASSERT_TRUE(state.changed);
  //int run_result = pm.run(module);
  //ASSERT_TRUE(EXIT_SUCCESS == run_result);
}
