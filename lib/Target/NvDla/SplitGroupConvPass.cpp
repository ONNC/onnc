//===- SplitGroupConvPass.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "SplitGroupConvPass.h"

#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Concat.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/Split.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/Transforms/Optimizations/OptimizationsUtils.h>

namespace onnc {

//===----------------------------------------------------------------------===//
// SplitGroupConvPass
//===----------------------------------------------------------------------===//
std::string  SplitGroupConvPass::splitPrefixName = "split_conv_s_";
std::string  SplitGroupConvPass::convPrefixName  = "split_conv_c_";
unsigned int SplitGroupConvPass::splitIdx        = 0;
unsigned int SplitGroupConvPass::convIdx         = 0;

Pass::ReturnType SplitGroupConvPass::runOnModule(Module& pModule)
{
  const Pass::ReturnType ret = BaseType::runOnModule(pModule);

  if (ret != kModuleNoChanged) {
    pModule.eraseUnusedValues();
  }

  return ret;
}

ComputeOperator* SplitGroupConvPass::genSplitConvsAndReturnConcat(ComputeGraph& pCG, Conv& pConv, Tensor* tensorX,
                                                                  Tensor* tensorW, Tensor* biasTensor)
{
  assert(pConv.getGroup().value() != 1);

  Tensor::Dimension oriChannel  = tensorX->getDimensions()[1];
  IntsAttr::Size    numOfSplits = static_cast<IntsAttr::Size>(pConv.getGroup().value());
  assert(oriChannel % numOfSplits == 0);
  assert(oriChannel / numOfSplits == tensorW->getDimensions()[1]);
  assert(tensorW->getDimensions()[0] % numOfSplits == 0);
  IntsAttr split(IntsAttr::VectorType(numOfSplits, oriChannel / numOfSplits));

  // 1. Generate a split operator
  Split* splitNode = pCG.addOperator<Split>(IntAttr(1), split);

  // remove tensorX original use in runOnModule
  splitNode->addInput(*tensorX);

  // 2. Generate split outputs and connect to concat,
  // 2.1 Generate a concat node
  Concat* concatNode = pCG.addOperator<Concat>(IntAttr(1)); // concat on c axis

  // 2.2 for each split out, create a path: split out(X) -> conv input -> conv out-> concat input
  //                                        slice W  /
  //                                        slice B  /
  int batch_channel = oriChannel / numOfSplits;
  int total_batch   = tensorW->getDimensions()[0];
  for (int id = 0; id < numOfSplits; id++) {
    Tensor* tensorXWB[3] = {nullptr, nullptr, nullptr};
    // 2.2.1 create X
    tensorXWB[0] = tensorX->create();
    tensorXWB[0]->setName(splitPrefixName + std::to_string(splitIdx++));
    tensorXWB[0] = pCG.addValue<Tensor>(tensorXWB[0]);
    assert(tensorXWB[0] != nullptr && "The name must be unique");
    Tensor::Dimensions xDims = tensorX->getDimensions();
    // We should split inputX On Channel
    xDims[1] = static_cast<Tensor::Dimension>(oriChannel / numOfSplits);
    tensorXWB[0]->setDimensions(xDims);
    splitNode->addOutput(*tensorXWB[0]);

    // 2.2.2 create W
    tensorXWB[1] = tensorW->create();
    tensorXWB[1]->setName(splitPrefixName + std::to_string(splitIdx++));
    tensorXWB[1] = pCG.addValue<Tensor>(tensorXWB[1]);
    assert(tensorXWB[1] != nullptr && "The name must be unique");
    // We should split inputW On Batch
    Tensor::Dimensions wDims = tensorW->getDimensions();
    wDims[0]                 = static_cast<Tensor::Dimension>(total_batch / numOfSplits);
    tensorXWB[1]->setDimensions(wDims);

    // copy weight
    auto* source = dynamic_cast<FloatTensor*>(tensorW);
    auto* weight = dynamic_cast<FloatTensor*>(tensorXWB[1]);
    assert(weight != nullptr and source != nullptr);

    int64_t           total          = source->getValues().size();
    int64_t           size_per_Batch = total / numOfSplits;
    Tensor::Dimension start          = id * size_per_Batch;
    Tensor::Dimension end            = start + size_per_Batch;
    for (size_t i = start; i < end; i++)
      weight->getValues().push_back(source->getValues()[i]);

    // create a Initializer for W
    Initializer* pISplit = pCG.addOperator<Initializer>();
    pISplit->addOutput(*tensorXWB[1]);

    // 2.2.3 create B
    if (biasTensor != nullptr) {
      tensorXWB[2] = biasTensor->create();
      tensorXWB[2]->setName(splitPrefixName + std::to_string(splitIdx++));
      tensorXWB[2] = pCG.addValue<Tensor>(tensorXWB[2]);
      assert(tensorXWB[2] != nullptr && "The name must be unique");
      assert(biasTensor->getNumOfDimensions() == 1);
      assert(biasTensor->getDimensions()[0] % numOfSplits == 0);
      Tensor::Dimensions bDims = biasTensor->getDimensions();
      bDims[0]                 = biasTensor->getDimensions()[0] / numOfSplits;
      tensorXWB[2]->setDimensions(bDims);

      // copy bias
      auto* source = dynamic_cast<FloatTensor*>(biasTensor);
      auto* bias   = dynamic_cast<FloatTensor*>(tensorXWB[2]);
      assert(bias != nullptr and source != nullptr);
      int total = biasTensor->getDimensions()[0];
      int batch = total / numOfSplits;
      int start = id * batch;
      int end   = start + batch;
      for (int i = start; i < end; i++)
        bias->getValues().push_back(source->getValues()[i]);

      // create a Initializer for B
      Initializer* pISplit = pCG.addOperator<Initializer>();
      pISplit->addOutput(*tensorXWB[2]);
    }

    // 2.2.3 create Conv node
    Conv* newConv = pCG.addOperator<Conv>(pConv.getAutoPad(), pConv.getDilations(), pConv.getGroup(),
                                          pConv.getKernelShape(), pConv.getPads(), pConv.getStrides());
    newConv->setGroup(1);
    newConv->addInput(*(tensorXWB[0]));
    newConv->addInput(*(tensorXWB[1]));
    if (tensorXWB[2] != nullptr)
      newConv->addInput(*(tensorXWB[2]));

    // 2.2.4 create conv output tensor
    Tensor* conv_out = tensorX->create();
    conv_out->setName(convPrefixName + std::to_string(convIdx++));
    conv_out = pCG.addValue<Tensor>(conv_out);
    assert(conv_out != nullptr && "The name must be unique");

    Tensor::Dimensions convDims = pConv.getOutput(0)->getDimensions();
    convDims[1]                 = total_batch / numOfSplits;
    conv_out->setDimensions(convDims);

    // 2.2.5 connect conv_out to concat
    newConv->addOutput(*conv_out);
    concatNode->addInput(*conv_out);
  }

  return concatNode;
}

Pass::ReturnType SplitGroupConvPass::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType ret = Pass::kModuleNoChanged;

  std::vector<ComputeOperator*> rmList; // Storing the ComputeOperators to be removed from the graph.
  for (ComputeOperator& node : pCG) {
    if (Conv* pConv = dyn_cast<Conv>(&node)) {
      // We are looking for Conv operators with attribute group > 1.
      if (pConv->getGroup().value() == 1) {
        continue;
      }

      Tensor* const inputXTensor = pConv->getX();
      Tensor* const inputWTensor = pConv->getW();

      Tensor* biasTensor = nullptr;
      if (node.getNumOfInputs() == 3)
        biasTensor = pConv->getB();
      assert(biasTensor == nullptr || biasTensor->getNumOfDimensions() == 1);

      ComputeOperator* pConcat = genSplitConvsAndReturnConcat(pCG, *pConv, inputXTensor, inputWTensor, biasTensor);

      // The output tensor of the Conv is re-routed to the output of the newly-created Concat.
      Value* outValue = node.getOutput(0);
      outValue->clearDefine();
      pConcat->addOutput(*outValue);

      node.removeAllInputs();
      rmList.emplace_back(&node);

      Tensor* checkRemoveTensor[2] = {inputXTensor, inputWTensor};
      for (unsigned idx = 0; idx < 2; ++idx) {
        if (checkRemoveTensor[idx]->getUses().empty()) {
          ComputeOperator* inputNode = static_cast<ComputeOperator*>(checkRemoveTensor[idx]->getDefine());
          assert(isa<Initializer>(inputNode));
          inputNode->removeAllOutputs();
          pCG.erase(*inputNode);
        }
      }
    }
  }

  for (auto* pNode : rmList) {
    pCG.erase(*pNode);
    ret |= Pass::kModuleChanged;
  }

  if (ret != kModuleNoChanged) {
    pCG.topologicalSort();
  }

  return ret;
}

} // namespace onnc
