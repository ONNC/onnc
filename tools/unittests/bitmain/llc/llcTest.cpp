#include <skypat/skypat.h>
#include <cstdlib>
#include "onnx/common/ir.h"
#include "TGOperator.h"
#include <memory>
#include <iostream>


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
  graph.addInitializer(tWeight1, "weight1");
  onnx::Tensor tBias1;
  graph.addInitializer(tBias1, "bias1");

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
  graph.addInitializer(tWeight2, "weight2");
  onnx::Tensor tBias2;
  graph.addInitializer(tBias2, "bias2");

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

  // register graph output
  graph.registerOutput(convOutVal2);

  std::unique_ptr<TGOperator> tgOp(TGOperator::makeTGOperator(*convNode1, 0));
  std::cout << "lowering: " << tgOp->getName()
            << ", offset: " << tgOp->getTotalWeightSize() << std::endl;

  ASSERT_EQ(139776, tgOp->getTotalWeightSize());
  std::unique_ptr<TGOperator> tgOp2(
      TGOperator::makeTGOperator(*convNode2, tgOp->getTotalWeightSize()));
  ASSERT_EQ(139776, tgOp2->getTotalWeightSize());
}

int main(int argc, char* argv[])
{
  skypat::Test::Initialize(&argc, argv);
  skypat::Test::RunAll();

  return (skypat::testing::UnitTest::self()->getNumOfFails() == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
