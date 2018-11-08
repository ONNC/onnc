//===- ProfilerTest.cpp ---------------------------------------------------===//
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
#include <onnc/IR/Compute/Profile.h>
#include <onnc/Support/IOStream.h>
#include <ostream>
#include <string>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Any Test
//===----------------------------------------------------------------------===//
SKYPAT_F(ProfilerTest, add_compute_op)
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

  Profile* op4 = builder.AddComputeOp<Profile>();
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 4);
  ASSERT_TRUE(op4->name().equals("Profile"));
  
  module.getComputeGraph("top-level")->erase(*op1);
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 3);

  module.getComputeGraph("top-level")->clear();
  ASSERT_EQ(module.getComputeGraph("top-level")->getNodeSize(), 0);
}

SKYPAT_F(ProfilerTest, print_before_and_after_bfs_search)
{
  onnc::Module module;
  IRBuilder builder(module);
  builder.CreateComputeGraph("top-level");

  ComputeOperator* op1 = builder.AddComputeOp<Conv>();
  ComputeOperator* op2 = builder.AddComputeOp<Relu>();
  ComputeOperator* op3 = builder.AddComputeOp<ATen>();
  ComputeOperator* op4 = builder.AddComputeOp<Abs>();
  ComputeOperator* op5 = builder.AddComputeOp<Conv>();
  Profile* opProfile = builder.AddComputeOp<Profile>();

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

  // Test Profiler
  opProfile->printAttributes(outs());
  opProfile->start();

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

  opProfile->stop();
  
  ASSERT_TRUE(i == builder.getComputeGraph()->bfs_end());

  // Test Profiler
  opProfile->printAttributes(outs());
}
