#include <skypat/skypat.h>
#include <cstdlib>
#include "onnx/common/ir.h"
#include "TGOperator.h"
#include <memory>
#include <iostream>
#include "TGBackend.h"
#include "reader_helper.h"

namespace {
  void initDim(std::vector<onnx::Dimension> *dims, int n, int c, int h, int w){
    dims->clear();
    dims->push_back(onnx::Dimension(n));
    dims->push_back(onnx::Dimension(c));
    dims->push_back(onnx::Dimension(h));
    dims->push_back(onnx::Dimension(w));
  }

  void initValue(onnx::Value *val, std::string name, int n, int c, int h,
                 int w, onnx::TensorProto_DataType elemType) {
    val->setUniqueName(name);
    std::shared_ptr<std::vector<onnx::Dimension> > inputDim =
        std::make_shared<std::vector<onnx::Dimension> >();
    initDim(inputDim.get(), n, c, h, w);
    val->setSizes(*inputDim.get());
    val->setElemType(elemType);
  }
}


SKYPAT_F(llcTest, testTotalWeightSize){
  onnx::Graph graph;

  // add intpu
  onnx::Value* input1 = graph.addInput();
  initValue(input1, "input", 1, 3, 227, 227, onnx::TensorProto_DataType_FLOAT);

  // add value
  onnx::Value* weight1 = graph.addInput();
  initValue(weight1, "weight1", 96, 3, 11, 11,
            onnx::TensorProto_DataType_FLOAT);

  onnx::Value* bias1 = graph.addInput();
  initValue(bias1, "bias1", 96, 1, 1, 1, onnx::TensorProto_DataType_FLOAT);

  // add addInitializer
  onnx::Tensor tWeight1;
  tWeight1.elem_type() = onnx::TensorProto_DataType_FLOAT;
  tWeight1.setName("tweight1");
  graph.addInitializer(tWeight1, "tweight1");
  onnx::Tensor tBias1;
  tBias1.elem_type() = onnx::TensorProto_DataType_FLOAT;
  tBias1.setName("tbias1");
  graph.addInitializer(tBias1, "tbias1");

  // create Conv node
  onnx::Node* convNode1 = graph.create(onnx::Symbol("Conv"));
  convNode1->addInput(input1);
  convNode1->addInput(weight1);
  convNode1->addInput(bias1);

  // init output
  onnx::Value* convOutVal1 = convNode1->outputs()[0];
  initValue(convOutVal1, "convOut1", 1, 96, 55, 55, onnx::TensorProto_DataType_FLOAT);
  graph.appendNode(convNode1);

  // add value
  onnx::Value* weight2 = graph.addInput();
  initValue(weight2, "weight2", 96, 3, 11, 11, onnx::TensorProto_DataType_FLOAT);

  onnx::Value* bias2 = graph.addInput();
  initValue(bias2, "bias2", 96, 1, 1, 1, onnx::TensorProto_DataType_FLOAT);

  // add addInitializer
  onnx::Tensor tWeight2;
  tWeight2.elem_type() = onnx::TensorProto_DataType_FLOAT;
  tWeight2.setName("tweight2");
  graph.addInitializer(tWeight2, "tweight2");

  onnx::Tensor tBias2;
  tBias2.elem_type() = onnx::TensorProto_DataType_FLOAT;
  tBias2.setName("tbias2");
  graph.addInitializer(tBias2, "tbias2");

  ////////////////////////////////////////////////////////////////////////
  // create Conv node
  onnx::Node* convNode2 = graph.create(onnx::Symbol("Conv"));
  convNode2->addInput(convOutVal1);
  convNode2->addInput(weight2);
  convNode2->addInput(bias2);

  // init output
  onnx::Value* convOutVal2 = convNode2->outputs()[0];
  initValue(convOutVal2, "convOut2", 1, 96, 55, 55, onnx::TensorProto_DataType_FLOAT);
  graph.appendNode(convNode2);

  ::updateOutputInfoPass::updateOutputInfo(graph);

  MemTable memTable;
  // plan global memory layout
  targetInfo::ddrScanAndAlloc(memTable, graph);

  // register graph output
  graph.registerOutput(convOutVal2);

  std::unique_ptr<TGOperator> tgOp(
      TGOperator::makeTGOperator(*convNode1, memTable));
  std::cout << "lowering: " << tgOp->getName()
            << ", offset: " << tgOp->getTotalWeightSize() << std::endl;

  ASSERT_EQ(618348, (memTable[weight1->uniqueName()] & (uint64_t)~(0x3)));
  std::unique_ptr<TGOperator> tgOp2(
      TGOperator::makeTGOperator(*convNode2, memTable));
  ASSERT_EQ(2059500, (memTable[convOutVal2->uniqueName()] & (uint64_t)~(0x3)));
}

SKYPAT_F(llcTest, testOutDims) {
  onnx::Graph graph;

  // add intpu
  onnx::Value *input1 = graph.addInput();
  initValue(input1, "input", 1, 3, 227, 227, onnx::TensorProto_DataType_FLOAT);

  // add value
  onnx::Value *weight1 = graph.addInput();
  initValue(weight1, "weight1", 96, 3, 11, 11,
            onnx::TensorProto_DataType_FLOAT);

  onnx::Value *bias1 = graph.addInput();
  initValue(bias1, "bias1", 96, 1, 1, 1, onnx::TensorProto_DataType_FLOAT);

  // add addInitializer
  onnx::Tensor tWeight1;
  graph.addInitializer(tWeight1, "weight1");
  onnx::Tensor tBias1;
  graph.addInitializer(tBias1, "bias1");

  // create Conv node
  onnx::Node *convNode1 = graph.create(onnx::Symbol("Conv"));
  convNode1->addInput(input1);
  convNode1->addInput(weight1);
  convNode1->addInput(bias1);

  {
    std::vector<int64_t> ints;
    for (int i = 0; i < 2; i++) {
      ints.push_back(11);
    }
    convNode1->is_(onnx::Symbol("kernel_shape"), std::move(ints));
  }

  {
    std::vector<int64_t> ints;
    for (int i = 0; i < 2; i++) {
      ints.push_back(4);
    }
    convNode1->is_(onnx::Symbol("strides"), std::move(ints));
  }

  // init output
  onnx::Value *convOutVal1 = convNode1->outputs()[0];
  graph.appendNode(convNode1);

  ////////////////////////////////////////////////////////////////////////
  // create MaxPool node
  onnx::Node *maxPoolNode2 = graph.create(onnx::Symbol("MaxPool"));
  maxPoolNode2->addInput(convOutVal1);
  {
    std::vector<int64_t> ints;
    for (int i = 0; i < 2; i++) {
      ints.push_back(3);
    }
    maxPoolNode2->is_(onnx::Symbol("kernel_shape"), std::move(ints));
  }

  {
    std::vector<int64_t> ints;
    for (int i = 0; i < 2; i++) {
      ints.push_back(2);
    }
    maxPoolNode2->is_(onnx::Symbol("strides"), std::move(ints));
  }

  // init output
  onnx::Value *maxPoolOutVal2 = maxPoolNode2->outputs()[0];
  graph.appendNode(maxPoolNode2);

  // register graph output
  graph.registerOutput(maxPoolOutVal2);

  // before update
  auto outConvValue = convNode1->outputs()[0];
  for (auto &i : outConvValue->sizes()) {
    std::cout << i.dim << ",";
  }
  std::cout << std::endl;
  auto outPoolValue = maxPoolNode2->outputs()[0];
  for (auto &i : outPoolValue->sizes()) {
    std::cout << i.dim << ",";
  }
  std::cout << std::endl;

  ASSERT_EQ(outConvValue->sizes().size(), 0);
  ASSERT_EQ(outPoolValue->sizes().size(), 0);

  ::updateOutputInfoPass::updateOutputInfo(graph);
  // after update
  std::vector<int64_t> outConvAns{ 1, 96, 55, 55 };
  std::vector<int64_t> outPoolAns{ 1, 96, 27, 27 };
  int idx = 0;
  for (auto &i : outConvValue->sizes()) {
    std::cout << i.dim << ",";
    ASSERT_EQ(outConvAns[idx++], i.dim);
  }
  std::cout << std::endl;
  idx = 0;
  for (auto &i : outPoolValue->sizes()) {
    std::cout << i.dim << ",";
    ASSERT_EQ(outPoolAns[idx++], i.dim);
  }
  std::cout << std::endl;
}

int main(int argc, char* argv[])
{
  skypat::Test::Initialize(&argc, argv);
  skypat::Test::RunAll();

  return (skypat::testing::UnitTest::self()->getNumOfFails() == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
