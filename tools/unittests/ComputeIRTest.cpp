//===- ComputeIRTest.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <onnc/Config/ONNX.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/OStrStream.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Scalar.h>
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
  errs() << alpha << std::endl;
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
  xTensor tensor2;
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

SKYPAT_F(ComputeIRTest, use_list_test)
{
  onnc::Module module;
  IRBuilder builder(module);

  ComputeGraph* cg = builder.CreateComputeGraph("top-level");
  ComputeOperator* op1 = builder.AddComputeOp<Conv>();

  onnc::Int8Tensor* i8t = cg->addValue<onnc::Int8Tensor>("val.i8");
  onnc::FloatTensor* ft = cg->addValue<onnc::FloatTensor>("val.f");

  op1->addInput(*i8t);
  i8t->replaceAllUsesWith(*ft);

  ASSERT_TRUE(i8t->getUses().empty());
  ASSERT_EQ(ft->getUses().size(), 1);
  ASSERT_EQ(ft->getUses()[0].getUser(), op1);
}

SKYPAT_F(ComputeIRTest, use_list_test2)
{
  onnc::Module module;
  IRBuilder builder(module);

  ComputeGraph* cg = builder.CreateComputeGraph("top-level");
  ComputeOperator* op1 = builder.AddComputeOp<Conv>();
  ComputeOperator* op2 = builder.AddComputeOp<Conv>();
  ComputeOperator* op3 = builder.AddComputeOp<Conv>();

  onnc::Int8Tensor* i8t = cg->addValue<onnc::Int8Tensor>("val.i8");
  onnc::FloatTensor* ft = cg->addValue<onnc::FloatTensor>("val.f");

  op1->addInput(*i8t);
  op2->addInput(*i8t);
  op3->addInput(*i8t);
  i8t->replaceAllUsesWith(*ft);

  ASSERT_TRUE(i8t->getUses().empty());
  ASSERT_EQ(ft->getUses().size(), 3);
  ASSERT_EQ(ft->getUses()[0].getUser(), op1);
  ASSERT_EQ(ft->getUses()[1].getUser(), op2);
  ASSERT_EQ(ft->getUses()[2].getUser(), op3);
}

SKYPAT_F(ComputeIRTest, replace_output_test)
{
  onnc::Module module;
  IRBuilder builder(module);

  ComputeGraph* cg = builder.CreateComputeGraph("top-level");
  onnc::Int8Tensor* i8t = cg->addValue<onnc::Int8Tensor>("val.i8");
  onnc::FloatTensor* ft = cg->addValue<onnc::FloatTensor>("val.f");

  ComputeOperator* conv = builder.AddComputeOp<Conv>();
  conv->addOutput(*ft);

  ComputeOperator* op1 = builder.AddComputeOp<Relu>();
  ComputeOperator* op2 = builder.AddComputeOp<Relu>();
  ComputeOperator* op3 = builder.AddComputeOp<Relu>();
  op1->addInput(*ft);
  op2->addInput(*ft);
  op3->addInput(*ft);

  Value* delVal = conv->getOutput(0);
  conv->replaceOutput(0, *i8t);
  cg->erase(*delVal);

  ASSERT_TRUE(ft->getUses().empty());
  ASSERT_EQ(i8t->getUses().size(), 3);
  ASSERT_EQ(i8t->getUses()[0].getUser(), op1);
  ASSERT_EQ(i8t->getUses()[1].getUser(), op2);
  ASSERT_EQ(i8t->getUses()[2].getUser(), op3);

  ASSERT_EQ(op1->getInput(0), i8t);
  ASSERT_EQ(op2->getInput(0), i8t);
  ASSERT_EQ(op3->getInput(0), i8t);
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
  xNode* conv = builder.AddNode("Conv",    {"data_0", "conv1_w_0", "conv1_b_0"});
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

  module.dump();
}
