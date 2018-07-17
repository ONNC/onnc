//===- TensorSelTest.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/Transforms/BookONNXGraphs.h>
#include <onnc/Transforms/PreTensorSel.h>
#include <onnc/Transforms/TensorSel.h>
#include <onnc/IR/IRBuilder.h>
#include <onnc/Core/PassManager.h>
#include <onnc/Transforms/DeadNodeElimination.h>
#include <onnc/Transforms/RemoveTrainingNodes.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Transforms/ComplementInputOperators.h>
#include <onnc/Transforms/ComplementInitializers.h>
#include <onnc/IRReader/ONNXReader.h>
#include <onnc/Transforms/ComplementOutputOperators.h>
#include <onnc/ADT/Rope.h>
#include <onnc/Support/OFStream.h>

#include <onnc/Transforms/TensorSel/Standards/ATenLower.h>
#include <onnc/Transforms/TensorSel/Standards/AveragePoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/SumLower.h>
#include <onnc/Transforms/TensorSel/Standards/AbsLower.h>
#include <onnc/Transforms/TensorSel/Standards/BatchNormalizationLower.h>
#include <onnc/Transforms/TensorSel/Standards/UpsampleLower.h>
#include <onnc/Transforms/TensorSel/Standards/AcosLower.h>
#include <onnc/Transforms/TensorSel/Standards/ConcatLower.h>
#include <onnc/Transforms/TensorSel/Standards/MaxPoolLower.h>
#include <onnc/Transforms/TensorSel/Standards/XorLower.h>
#include <onnc/Transforms/TensorSel/Standards/AddLower.h>
#include <onnc/Transforms/TensorSel/Standards/ConvLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReluLower.h>
#include <onnc/Transforms/TensorSel/Standards/AffineLower.h>
#include <onnc/Transforms/TensorSel/Standards/GemmLower.h>
#include <onnc/Transforms/TensorSel/Standards/ReshapeLower.h>
#include <onnc/Transforms/TensorSel/Standards/AndLower.h>
#include <onnc/Transforms/TensorSel/Standards/LRNLower.h>
#include <onnc/Transforms/TensorSel/Standards/SoftmaxLower.h>

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
  pm.add(CreateComplementInitializers(), state);
  pm.add(CreateComplementInputOperators(), state);

  /// create tensor selection
  TensorSel* tensor_selection = new TensorSel();
  tensor_selection->getLowerRegistry().emplace<onnc::ATenLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::AveragePoolLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::SumLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::AbsLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::BatchNormalizationLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::UpsampleLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::AcosLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::ConcatLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::MaxPoolLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::XorLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::AddLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::ConvLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::ReluLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::AffineLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::GemmLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::ReshapeLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::AndLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::LRNLower>();
  tensor_selection->getLowerRegistry().emplace<onnc::SoftmaxLower>();
  pm.add(tensor_selection, state);
  pm.add(CreateComplementOutputOperators(), state);

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

  // ComplementInitializers
  pm.step(module, state);
  {
    errs() << state.pass->getPassName() << std::endl;
    OFStream os((Rope(++counter) + Rope(".") +
                 Rope(state.pass->getPassName()) + (".log")).str());
    module.print(os);
  }

  // ComplementInputOperators
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

  // ComplementOutputOperators
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
  builder.AddNode("Conv",    {"data_0", "conv1_w_0", "conv1_b_0"});
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

  pm.add(CreateRemoveTrainingNodesPass(), state);
  pm.add(CreateUpdateGraphOutputSizePass(), state);
  pm.add(CreatePreTensorSel(), state);

  /// create tensor selection
  TensorSel* tensor_selection = new TensorSel();
  pm.add(tensor_selection, state);

  // RemoveTrainingNodesPass
  pm.step(module, state);
  errs() << state.pass->getPassName() << std::endl;

  // UpdateGraphOutputSizePass
  pm.step(module, state);
  errs() << state.pass->getPassName() << std::endl;

  // PreTensorSel
  pm.step(module, state);
  errs() << state.pass->getPassName() << std::endl;

  // TensorSel
  //pm.step(module, state);
  //errs() << state.pass->getPassName() << std::endl;

  //ASSERT_TRUE(state.execution.empty());
  //ASSERT_TRUE(state.changed);
  //int run_result = pm.run(module);
  //ASSERT_TRUE(EXIT_SUCCESS == run_result);
}
