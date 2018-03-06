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

  // add value
  onnx::Value* vInput = graph.addInput();
  initValue(vInput, "input", 1, 3, 227, 227, onnx::TensorProto_DataType_FLOAT);

  onnx::Value* vWeight = graph.addInput();
  initValue(vWeight, "weight", 96, 3, 11, 11, onnx::TensorProto_DataType_FLOAT);

  onnx::Value* vBias = graph.addInput();
  initValue(vBias, "bias", 96, 1, 1, 1, onnx::TensorProto_DataType_FLOAT);

  // add addInitializer
  onnx::Tensor tWeight;
  graph.addInitializer(tWeight, "weight");
  onnx::Tensor tBias;
  graph.addInitializer(tBias, "bias");

  onnx::Node* convNode = graph.create(onnx::Symbol("Conv"));
  convNode->addInput(vInput);
  convNode->addInput(vWeight);
  convNode->addInput(vBias);
  graph.appendNode(convNode);
  onnx::Value* convOutVal = convNode->outputs()[0];
  initValue(convOutVal, "bias", 1, 96, 55, 55, onnx::TensorProto_DataType_FLOAT);
  graph.registerOutput(convOutVal);

  std::unique_ptr<TGOperator> tgOp(TGOperator::makeTGOperator(*convNode, 0));
  std::cout << "lowering: " << tgOp->getName()
            << ", offset: " << tgOp->getTotalWeightSize() << std::endl;
  ASSERT_EQ(139776, tgOp->getTotalWeightSize());
}

int main(int argc, char* argv[])
{
  skypat::Test::Initialize(&argc, argv);
  skypat::Test::RunAll();

  return (skypat::testing::UnitTest::self()->getNumOfFails() == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
