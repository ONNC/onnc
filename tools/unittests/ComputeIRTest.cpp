//===- ComputeIRTest.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/OStrStream.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Scalar.h>
#include <onnx/common/tensor.h>
#include <onnc/IR/IRBuilder.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/Compute/ATen.h>
#include <onnc/IR/Compute/Abs.h>
#include <onnc/Support/IOStream.h>
#include <ostream>
#include <string>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Any Test
//===----------------------------------------------------------------------===//
SKYPAT_F(ComputeIRTest, attr_test)
{
  FloatAttr alpha;
  ASSERT_EQ(alpha.kind(), Attribute::kFloat);
}

SKYPAT_F(ComputeIRTest, scalar_test)
{
  Scalar a;

  ASSERT_EQ(a.kind(), Value::kUndefined);

  Int32Scalar b;

  ASSERT_EQ(b.kind(), Value::kInt32);
  ASSERT_EQ(b.getValue(), 0);

  b.setValue(10);
  ASSERT_EQ(b.getValue(), 10);

  Int64Scalar c;
  ASSERT_EQ(c.kind(), Value::kInt64);

  // string
  ::onnx::Tensor tensor2;
  StringScalar e(tensor2);
  ASSERT_TRUE(e.empty());
  ASSERT_EQ(e.kind(), Value::kString);
  e.setValue("test");
  ASSERT_TRUE(e.getValue() == "test");
}

SKYPAT_F(ComputeIRTest, tensor_test)
{
  Tensor a;
  ASSERT_EQ(a.kind(), Value::kUndefined);

  Int32Tensor b;
  ASSERT_EQ(b.kind(), Value::kInt32);
  ASSERT_EQ(b.getValues().size(), 0);
}

SKYPAT_F(ComputeIRTest, add_compute_op)
{
  onnc::Module module;
  IRBuilder builder(module);

  builder.CreateComputeGraph("top-level");
  ComputeOperator* op1 = builder.AddComputeOp<Conv>();
  ASSERT_TRUE(op1->name().equals("Conv"));

  /// builder.getComputeGraph()
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 1);

  ComputeOperator* op2 = builder.AddComputeOp<Relu>();
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 2);
  ASSERT_TRUE(op2->name().equals("Relu"));

  ATen* op3 = builder.AddComputeOp<ATen>();
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 3);
  ASSERT_TRUE(op3->name().equals("ATen"));
  
  module.getComputeGraph("top-level")->erase(*op1);
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 2);

  module.getComputeGraph("top-level")->clear();
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 0);
}

SKYPAT_F(ComputeIRTest, add_compute_opnd)
{
  onnc::Module module;
  IRBuilder builder(module);

  builder.CreateComputeGraph("top-level");
  ComputeOperator* op1 = builder.AddComputeOp<Conv>();
  ASSERT_TRUE(op1->name().equals("Conv"));

  // builder.getComputeGraph() can do the same module.getComputeGraph("name");
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 1);

  ComputeOperator* op2 = builder.AddComputeOp<Relu>();
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 2);
  ASSERT_TRUE(op2->name().equals("Relu"));

  ATen* op3 = builder.AddComputeOp<ATen>();
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 3);
  ASSERT_TRUE(op3->name().equals("ATen"));
  
  ComputeMemOperand* opnd1
      = builder.AddComputeOpnd<ComputeMemOperand>(*op1, *op2, 0x11, 20);

  EXPECT_TRUE(builder.getComputeGraph()->name() == "top-level");
  ASSERT_EQ(opnd1->start(), 0x11);
  ASSERT_EQ(opnd1->length(), 20);
  ASSERT_EQ(builder.getComputeGraph()->getArcSize(), 1);

  module.getComputeGraph("top-level")->clear();
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 0);
}

SKYPAT_F(ComputeIRTest, bfs_search)
{
  onnc::Module module;
  IRBuilder builder(module);
  builder.CreateComputeGraph("top-level");

  ComputeOperator* op1 = builder.AddComputeOp<Conv>();
  ComputeOperator* op2 = builder.AddComputeOp<Relu>();
  ComputeOperator* op3 = builder.AddComputeOp<ATen>();
  ComputeOperator* op4 = builder.AddComputeOp<Abs>();
  ComputeOperator* op5 = builder.AddComputeOp<Conv>();

  //    op1       BFS: op1 - op2 - op3 - op4 - op5
  //   /   \      DFS: op1 - op2 - op3 - op5 - op4
  //  op2->op3
  //  |     |
  //  op4  op5
  builder.AddComputeOpnd<ComputeMemOperand>(*op1, *op2);
  builder.AddComputeOpnd<ComputeMemOperand>(*op1, *op3);
  builder.AddComputeOpnd<ComputeMemOperand>(*op2, *op3);
  builder.AddComputeOpnd<ComputeMemOperand>(*op2, *op4);
  builder.AddComputeOpnd<ComputeMemOperand>(*op3, *op5);

  ComputeGraph::bfs_iterator i;
  i = builder.getComputeGraph()->bfs_begin();
  ASSERT_TRUE(i->name() == op1->name());
  i.next();
  ASSERT_TRUE(i->name() == op2->name());
  i.next();
  ASSERT_TRUE(i->name() == op3->name());
  i.next();
  ASSERT_TRUE(i->name() == op4->name());
  i.next();
  ASSERT_TRUE(i->name() == op5->name());
  i.next();
  ASSERT_TRUE(i == builder.getComputeGraph()->bfs_end());
}

SKYPAT_F(ComputeIRTest, dfs_search)
{
  onnc::Module module;
  IRBuilder builder(module);
  builder.CreateComputeGraph("top-level");

  ComputeOperator* op1 = builder.AddComputeOp<Conv>();
  ComputeOperator* op2 = builder.AddComputeOp<Relu>();
  ComputeOperator* op3 = builder.AddComputeOp<ATen>();
  ComputeOperator* op4 = builder.AddComputeOp<Abs>();
  ComputeOperator* op5 = builder.AddComputeOp<Conv>();

  //    op1       BFS: op1 - op2 - op3 - op4 - op5
  //   /   \      DFS: op1 - op2 - op3 - op5 - op4
  //  op2->op3
  //  |     |
  //  op4  op5
  builder.AddComputeOpnd<ComputeMemOperand>(*op1, *op2);
  builder.AddComputeOpnd<ComputeMemOperand>(*op1, *op3);
  builder.AddComputeOpnd<ComputeMemOperand>(*op2, *op3);
  builder.AddComputeOpnd<ComputeMemOperand>(*op2, *op4);
  builder.AddComputeOpnd<ComputeMemOperand>(*op3, *op5);


  ComputeGraph::dfs_iterator i;
  i = builder.getComputeGraph()->dfs_begin();
  ASSERT_TRUE(i->name() == op1->name());
  i.next();
  ASSERT_TRUE(i->name() == op2->name());
  i.next();
  ASSERT_TRUE(i->name() == op3->name());
  i.next();
  ASSERT_TRUE(i->name() == op5->name());
  i.next();
  ASSERT_TRUE(i->name() == op4->name());
  i.next();
  ASSERT_TRUE(i == builder.getComputeGraph()->dfs_end());
}

SKYPAT_F(ComputeIRTest, deep_clone)
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

  ::onnx::Node* copy = builder.DeepClone(*conv);
  builder.FinalizeTensorGraph({"prob_1"});

  // instance is a copy
  ASSERT_FALSE(copy == conv);

  // all inputs should be identical
  ASSERT_EQ(copy->inputs().size(), conv->inputs().size());
  ASSERT_TRUE(copy->inputs()[0] == conv->inputs()[0]);
  ASSERT_TRUE(copy->inputs()[1] == conv->inputs()[1]);
  ASSERT_TRUE(copy->inputs()[2] == conv->inputs()[2]);

  // all outputs should be copies
  ASSERT_TRUE(copy->outputs().size() == conv->outputs().size());
  ASSERT_TRUE(copy->outputs()[0] != conv->outputs()[0]);

  module.dump();
}
