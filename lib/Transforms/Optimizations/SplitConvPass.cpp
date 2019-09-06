//===- SplitConvPass.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/Split.h>
#include <onnc/IR/Compute/Sum.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/Transforms/Optimizations/OptimizationsUtils.h>
#include <onnc/Transforms/Optimizations/SplitConvPass.h>

namespace onnc {

//===----------------------------------------------------------------------===//
// SplitConvPass
//===----------------------------------------------------------------------===//
std::string  SplitConvPass::splitPrefixName = "split_conv_s_";
std::string  SplitConvPass::convPrefixName  = "split_conv_c_";
unsigned int SplitConvPass::splitIdx        = 0;
unsigned int SplitConvPass::convIdx         = 0;

Pass::ReturnType SplitConvPass::runOnModule(Module& pModule)
{
  const Pass::ReturnType ret = BaseType::runOnModule(pModule);

  if (ret != kModuleNoChanged) {
    pModule.eraseUnusedValues();
  }

  return ret;
}

ComputeOperator* SplitConvPass::genSplitConvsAndReturnSum(ComputeGraph& pCG, Conv& pConv, Tensor& tensorX,
                                                          Tensor& tensorW, Tensor* biasTensor)
{
  assert(pConv.getGroup().value() == 1);

  const Tensor::Dimension maxChannel = m_MaxNumOfConvChannelsGetter(pConv);
  assert(maxChannel != -1);

  Tensor::Dimension oriChannel  = tensorX.getDimensions()[1];
  IntsAttr::Size    numOfSplits = static_cast<IntsAttr::Size>((oriChannel + maxChannel - 1) / maxChannel);
  IntsAttr          split(IntsAttr::VectorType(numOfSplits, maxChannel));
  if (oriChannel % maxChannel != 0) {
    split.vector()[numOfSplits - 1] = oriChannel % maxChannel;
  }

  // Split on the second dim (N, C, H, W), on C
  Tensor* splitTensors[2] = {&tensorX, &tensorW};
  Split*  splitNodes[2]   = {nullptr, nullptr};
  for (unsigned idx = 0; idx < 2; ++idx) {
    // There is no split node if the input is an Initializer
    if (!isa<Initializer>(static_cast<ComputeOperator*>(splitTensors[idx]->getDefine()))) {
      splitNodes[idx] = pCG.addOperator<Split>(IntAttr(1), split);
      splitNodes[idx]->addInput(*splitTensors[idx]);
    }
  }

  ComputeOperator*    outputSum   = pCG.addOperator<Sum>();
  IntsAttr::ValueType baseChannel = 0;
  bool                attachBias  = false;
  for (const auto& curChannel : split.vector()) {
    Tensor* tensorXW[2];
    // Create one tensor for each split output
    for (unsigned idx = 0; idx < 2; ++idx) {
      Tensor* splitTensor = splitTensors[idx]->create();
      splitTensor->setName(splitPrefixName + std::to_string(splitIdx));
      ++splitIdx;
      splitTensor = pCG.addValue<Tensor>(splitTensor);
      assert(splitTensor != nullptr && "The name must be unique");

      Tensor::Dimensions splitDims = splitTensors[idx]->getDimensions();
      splitDims[1]                 = static_cast<Tensor::Dimension>(curChannel);
      splitTensor->setDimensions(splitDims);

      if (splitNodes[idx] != nullptr) {
        splitNodes[idx]->addOutput(*splitTensor);
      } else {
        internal::visitTensorWithFunc<PP_UNWRAP(ALL_TENSORTYPE_LIST)>(
          splitTensors[idx],
          internal::ApplySplitOnChannel(splitTensor, Tensor::Size(1), baseChannel, baseChannel + curChannel));
        Initializer* pISplit = pCG.addOperator<Initializer>();
        pISplit->addOutput(*splitTensor);
      }
      tensorXW[idx] = splitTensor;
    }

    // Create new Conv
    ComputeOperator* newConv = pCG.addOperator<Conv>(pConv.getAutoPad(), pConv.getDilations(), pConv.getGroup(),
                                                     pConv.getKernelShape(), pConv.getPads(), pConv.getStrides());
    newConv->addInput(*(tensorXW[0]));
    newConv->addInput(*(tensorXW[1]));

    if (biasTensor != nullptr && (!attachBias)) {
      attachBias = true;
      newConv->addInput(*biasTensor);
    }

    // Create Conv output tensor
    Tensor* convTensor = tensorX.create();
    convTensor->setName(convPrefixName + std::to_string(convIdx));
    ++convIdx;
    convTensor = pCG.addValue<Tensor>(convTensor);
    assert(convTensor != nullptr && "The name must be unique");

    Tensor::Dimensions convDims = pConv.getOutput(0)->getDimensions();
    convTensor->setDimensions(convDims);

    newConv->addOutput(*convTensor);
    outputSum->addInput(*convTensor);
    baseChannel += curChannel;
  }

  return outputSum;
}

Pass::ReturnType SplitConvPass::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType ret = Pass::kModuleNoChanged;

  std::vector<ComputeOperator*> rmList;
  for (ComputeOperator& node : pCG) {
    if (Conv* pConv = dyn_cast<Conv>(&node)) {
      assert((node.getNumOfInputs() == 2 || node.getNumOfInputs() == 3) &&
             "Conv must have exactly two or three inputs");
      assert(node.getNumOfOutputs() == 1 && "Conv must have exactly one output");

      // donot support split group convolutions here
      if (pConv->getGroup().value() != 1) {
        continue;
      }

      Tensor* const inputXTensor = pConv->getX();
      Tensor* const inputWTensor = pConv->getW();

      assert(inputXTensor->getNumOfDimensions() == 4);
      assert(inputWTensor->getNumOfDimensions() == 4);

      Tensor* biasTensor = nullptr;
      if (node.getNumOfInputs() == 3)
        biasTensor = pConv->getB();
      assert(biasTensor == nullptr || biasTensor->getNumOfDimensions() == 1);

      // Whether we need to split Conv, only support for Conv with group num = 1
      if (pConv->getGroup().value() == 1 && inputXTensor->getDimensions()[1] > m_MaxNumOfConvChannelsGetter(*pConv)) {
        // cannot split this Conv due to cannot put data & weight into C_BUF by any combination,
        // so we ignore it in this pass
        if (m_MaxNumOfConvChannelsGetter(*pConv) == -1) {
          continue;
        }

        ComputeOperator* pSum = genSplitConvsAndReturnSum(pCG, *pConv, *inputXTensor, *inputWTensor, biasTensor);

        Value* outValue = node.getOutput(0);
        outValue->clearDefine();
        pSum->addOutput(*outValue);

        // Do not removeAllOutputs here, because the output Value
        // has been clear define
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
