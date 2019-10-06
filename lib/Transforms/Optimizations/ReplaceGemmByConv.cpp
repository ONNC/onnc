//===- ReplaceGemmByConv.cpp ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Flatten.h>
#include <onnc/IR/Compute/Gemm.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/Mul.h>
#include <onnc/IR/Compute/Squeeze.h>
#include <onnc/IR/Compute/Sum.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/Compute/Unsqueeze.h>
#include <onnc/Transforms/Optimizations/OptimizationsUtils.h>
#include <onnc/Transforms/Optimizations/ReplaceGemmByConv.h>

using namespace onnc;

const static std::unordered_set<const void *>
  shapingNodeIDs{&Squeeze::ID, &Unsqueeze::ID, &Reshape::ID, &Flatten::ID};

static Initializer* checkReshapeAndReturnInitializer(ComputeOperator* pNode)
{
  if (Reshape* const pR = dyn_cast<Reshape>(pNode)) {
    Tensor* const          shapeTensor = pR->getShape();
    ComputeOperator* const shapeNode   = static_cast<ComputeOperator*>(shapeTensor->getDefine());
    if (!isa<Initializer>(shapeNode)) {
      assert(0 && "Now only support input shape is Initializer, and this \
assertion is only used to check this condition. This assertion can be removed \
if the output shape can be derived during compilation time. But there will be \
dangling nodes that should be eliminated by another pass.");
    } else {
      return dyn_cast<Initializer>(shapeNode);
    }
  }
}

static void checkReshape(const ComputeOperator* pNode)
{
  if (const Reshape* const pR = dyn_cast<Reshape>(pNode)) {
    const Tensor* const          shapeTensor = pR->getShape();
    const ComputeOperator* const shapeNode   = static_cast<const ComputeOperator*>(shapeTensor->getDefine());
    if (isa<Initializer>(shapeNode)) {
    }
  }
}

static void removeReshapeBeforeReLU(ComputeGraph& pCG, Tensor& curTensor) {
  Tensor* inputTensor = &curTensor;
  ComputeOperator* inputNode = static_cast<ComputeOperator*>(inputTensor->getDefine());
  if (isa<Relu>(inputNode)) {
    assert(inputTensor->getUses().size() == 1 &&
           "Output of Relu can only have one Use to get through");
    Tensor* tmpTensor = static_cast<Tensor*>(inputNode->getInput(0));
    ComputeOperator* tmpNode = static_cast<ComputeOperator*>(tmpTensor->getDefine());
    if (isa<Reshape>(tmpNode)) {
      assert(tmpTensor->getUses().size() == 1 &&
             "Output of Reshape can only have one Use to get through");
      auto pShape = checkReshapeAndReturnInitializer(tmpNode);
      if (pShape != nullptr) {
        pCG.erase(*pShape);
      }

      // The Tensor before Reshape
      Tensor* oriTensor = static_cast<Tensor*>(tmpNode->getInput(0));
      assert(oriTensor->getNumOfDimensions() == 4 &&
             "Original tensor before Relu and Reshape must have four dimensions");
      inputTensor->setDimensions(oriTensor->getDimensions());

      tmpTensor->replaceAllUsesWith(*oriTensor);
      tmpNode->removeAllInputs();
      tmpNode->removeAllOutputs();
      pCG.erase(*tmpNode);
    }
  }
}

// the shaping nodes should form a singly linked list
// inputs to shaping nodes might be more than one (Reshape)
static Tensor* getTensorBeforeShaping(ComputeGraph& pCG, Tensor& curTensor) {
  Tensor* inputTensor = &curTensor;
  ComputeOperator* inputNode;
  do {
    inputNode = static_cast<ComputeOperator*> (inputTensor->getDefine());

    if (inputTensor->getUses().size() == 1 &&
        shapingNodeIDs.count(inputNode->getID())) {

      assert(inputNode->getNumOfInputs() >= 1 &&
             "Shaping node must have no less than one input");

      // Do conditional check for Reshape
      checkReshape(inputNode);

      // Assume input value always at index 0
      // Currently true for all shaping nodes.
      inputTensor = static_cast<Tensor*>(inputNode->getInput(0));
    }
    else break;

  } while(true);

  // Special Case:
  // Before: -> Reshape -> ReLU ->(Now Here) ... -> Gemm
  // After:  -> ReLU ->(Now Here) ... -> Gemm
  // The dimensions of inputTensor might change
  removeReshapeBeforeReLU(pCG, *inputTensor);
  
  return inputTensor;
}

static void eraseNodesBetween(ComputeGraph& pCG, Tensor* startTensor, Tensor* endTensor) {
  // use a count to prevent while loop failure
  unsigned cnt = 0;
  while (startTensor != endTensor) {
    ++cnt;

    assert(startTensor->getUses().size() == 1);
    ComputeOperator* pNode = startTensor->getUses()[0].getUser();

    assert(pNode->getNumOfOutputs() == 1);
    startTensor = static_cast<Tensor*>(pNode->getOutput(0));

    // If there are more than one inputs, check they are Initializer and erase them
    for (unsigned int idx = 1; idx < pNode->getNumOfInputs(); ++idx) {
      Value* inValue = pNode->getInput(idx);
      if (inValue->getUses().size() == 1) {
        ComputeOperator* inNode = static_cast<ComputeOperator*>(inValue->getDefine());
        assert(isa<Initializer>(inNode) &&
               "Current implementation only support elimination of Initializer");
        inNode->removeAllOutputs();
        pCG.erase(*inNode);
      }
    }

    pNode->removeAllInputs();
    pNode->removeAllOutputs();
    pCG.erase(*pNode);

    if (cnt == 1000000) assert(0 && "Too many times of while loop...");
  }
}

//===----------------------------------------------------------------------===//
// ReplaceGemmByConv
//===----------------------------------------------------------------------===//
const std::string ReplaceGemmByConv::convPrefixName = "replace_gemm_by_conv_";
const std::string ReplaceGemmByConv::reshapePrefixName = "replace_gemm_by_conv_reshape_";
const std::string ReplaceGemmByConv::shapePrefixName = "replace_gemm_by_conv_shape_";

unsigned ReplaceGemmByConv::convIdx = 0;
unsigned ReplaceGemmByConv::reshapeIdx = 0;
unsigned ReplaceGemmByConv::shapeIdx = 0;

// Add Reshape before inputTensor, and Reshape inputTensor
// to 4 dimensional tensors. Return new tensor.
Tensor* ReplaceGemmByConv::addReshapeBefore(
    ComputeGraph& pCG, Tensor* inputTensor) {
  assert(inputTensor->getUses().size() == 1);
  Initializer* pShape = pCG.addOperator<Initializer>();
  Int64Tensor* shapeTensor = pCG.addValue<Int64Tensor>(shapePrefixName + std::to_string(shapeIdx));
  ++shapeIdx;
  assert(shapeTensor != nullptr && "The name must be unique");

  const Tensor::Dimensions& originalDims = inputTensor->getDimensions();
  Tensor::Dimensions newDims(4, 0);

  Tensor::Size n = originalDims.size();
  for (Tensor::Size idx = 0; idx < std::min(n, Tensor::Size(4)); ++idx) {
    newDims[idx] = originalDims[idx];
  }

  if (n < 4) {
    for (Tensor::Size idx = n; idx < 4; ++idx) {
      newDims[idx] = 1;
    }
  }
  else if (n > 4) {
    for (Tensor::Size idx = 4; idx < n; ++idx) {
      newDims[3] *= originalDims[idx];
    }
  }
  
  assert(newDims.size() == 4);
  shapeTensor->setDimensions({4});
  shapeTensor->getValues() = Int64Tensor::ValueList(newDims);
  pShape->setTensor(*shapeTensor);

  // Create output Tensor here
  Tensor* reshapedTensor = inputTensor->create();
  reshapedTensor->setName(reshapePrefixName + std::to_string(reshapeIdx));
  ++reshapeIdx;
  reshapedTensor = pCG.addValue<Tensor>(reshapedTensor);
  assert(reshapedTensor != nullptr && "The name must be unique");

  reshapedTensor->setDimensions(newDims);
  inputTensor->replaceAllUsesWith(*reshapedTensor);

  Reshape* pR = pCG.addOperator<Reshape>();
  pR->addInput(*inputTensor);
  pR->addInput(*shapeTensor);
  pR->addOutput(*reshapedTensor);

  return reshapedTensor;
}

// Now the implementation of Conv only support tensor of num of dimensions == 4
ComputeOperator* ReplaceGemmByConv::constructConvAndReshape(
    ComputeGraph& pCG, Tensor* tA, Tensor* tB, Tensor* tC) {
  // tA: (1, C, d0, d1) -> 4 dims, Ex. (1, 256, 6, 6) swim (N, C, d0, d1)
  // tB: (M, C x d0 x d1) -> 2 dims, Ex. (4096, 256x6x6)
  Tensor::Dimensions dimsA = tA->getDimensions();
  assert(dimsA.size() == 4 && "The num of dimensions must be equal to 4. \
The generalized case requires another Squeeze/Unsqueeze and all unmatched dims == 1");

  Tensor::Dimension total = 1;
  for (const auto& dim : dimsA) total *= dim;
  
  Tensor::Dimension N = dimsA[0];//swim: with batch size, tA is (N, C, d0, d1)


  assert(tB->getNumOfDimensions() == 2);
  assert(total/N == tB->getDimensions()[1]);
  //swim: tB is (M, C x d0 x d1), tA is (N, C, d0, d1) and total=NxCxd0xd1
  //swim: so total/N=Cxd0xd1=tB->getDimensions()[1]

  // tB: (M, C, d0, d1, ...) -> n dims
  Tensor::Dimension M = tB->getDimensions()[0];
  dimsA[0] = M;
  tB->setDimensions(dimsA);

  // (1), (1, 1), (M), (1, M) to (M)
  internal::visitTensorWithFunc<PP_UNWRAP(ALL_TENSORTYPE_LIST)>(
      tC, internal::BroadcastingTo1D(M));

  // tA: (1, C, d0, d1) swim (N, C, d0, d1)
  // tB: (M, C, d0, d1)
  // tC: (M)

  // Create Conv here
  Tensor::Size n = dimsA.size(); // n == 4 here
  IntsAttr pKernelShape; // kernel shape: (d0, d1)

  for (Tensor::Size idx = 2; idx < n; ++idx) {
    pKernelShape.vector().emplace_back(dimsA[idx]);
  }
  IntsAttr pPads(IntsAttr::VectorType(2*(n-2), 0));
  // kernel_shape and the Gemm input A's last 2 dimensions are all same,
  // so the strides are not used in this case. pass strides = [1, 1] to
  // construct Conv here
  const IntsAttr strides(IntsAttr::VectorType(2, 1));

  ComputeOperator* pConv = pCG.addOperator<Conv>(StringAttr("NOTSET"), 
                                                 IntsAttr(IntsAttr::VectorType(2, 1)),
                                                 IntAttr(1),
                                                 pKernelShape,
                                                 pPads,
                                                 strides);
  pConv->addInput(*tA);
  pConv->addInput(*tB);
  pConv->addInput(*tC);

  // Create Reshape
  Tensor* convTensor = tA->clone();
  convTensor->setName(convPrefixName + std::to_string(convIdx));
  ++convIdx;
  convTensor = pCG.addValue<Tensor>(convTensor);
  assert(convTensor != nullptr && "The name must be unique");

  internal::visitTensorWithFunc<PP_UNWRAP(ALL_TENSORTYPE_LIST)>(
      convTensor, internal::ClearTensorValues());
  convTensor->setDimensions({N, M, 1, 1});//swim: with batch size, the first dim is N
  pConv->addOutput(*convTensor);

  Initializer* pShape = pCG.addOperator<Initializer>();
  Int64Tensor* shapeTensor = pCG.addValue<Int64Tensor>(shapePrefixName + std::to_string(shapeIdx));
  ++shapeIdx;
  assert(shapeTensor != nullptr && "The name must be unique");

  shapeTensor->setDimensions({2});
  shapeTensor->getValues() = Int64Tensor::ValueList({N, M});//swim: with batch size, the first dim is N
  pShape->setTensor(*shapeTensor);

  Reshape* pR = pCG.addOperator<Reshape>();
  pR->addInput(*convTensor);
  pR->addInput(*shapeTensor);

  return pR;
}

Pass::ReturnType ReplaceGemmByConv::runOnModule(Module& pModule)
{
  const Pass::ReturnType ret = BaseType::runOnModule(pModule);

  if (ret != kModuleNoChanged) {
    pModule.eraseUnusedValues();
  }

  return ret;
}

Pass::ReturnType ReplaceGemmByConv::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType ret = Pass::kModuleNoChanged;

  std::vector<ComputeOperator*> rmList;
  for (ComputeOperator &node : pCG) {
    if (Gemm* pG = dyn_cast<Gemm>(&node)) {
      assert(node.getNumOfInputs() == 3 && "Gemm must have exactly three inputs");
      assert(node.getNumOfOutputs() == 1 && "Gemm must have exactly one output");

      Tensor* const inputATensor = pG->getA();
      Tensor* const inputBTensor = pG->getB();
      Tensor* const inputCTensor = pG->getC();

      assert(inputATensor->getNumOfDimensions() == 2);
      assert(inputBTensor->getNumOfDimensions() == 2);

#define TEMP_ASSERT() do { assert(0 && "This Gemm will remain on the graph, and now it is \
not allowed. This assertion should be removed after general support of Gemm \
has been done."); } while(false)

      if (inputATensor->getUses().size() > 1 ||
          inputBTensor->getUses().size() > 1 ||
          inputCTensor->getUses().size() > 1) {
        TEMP_ASSERT();
        continue;
      }

      if (pG->getTransA().value() != 0 ){
        //swim:batch size may not 1, so not check inputATensor->getDimensions()[0] equal 1 or not
        TEMP_ASSERT();
        continue;
      }

      ComputeOperator* inputA, * inputB, * inputC;
      inputA = dynamic_cast<ComputeOperator*>(inputATensor->getDefine());
      inputB = dynamic_cast<ComputeOperator*>(inputBTensor->getDefine());
      inputC = dynamic_cast<ComputeOperator*>(inputCTensor->getDefine());

      if (!isa<Initializer>(inputB) ||
          !isa<Initializer>(inputC)) {
        TEMP_ASSERT();
        continue;
      }

#undef TEMP_ASSERT

      // apply transpose to B if there isn't one, so we can get
      // Tensor A: (1, K), Tensor B: (M, K) swim:(N, K)
      if (pG->getTransB().value() == 0)  {
        internal::visitTensorWithFunc<PP_UNWRAP(ALL_TENSORTYPE_LIST)>(
            inputBTensor, internal::ApplyTransOn2DTensor());
      }
      assert(inputATensor->getDimensions()[1] == inputBTensor->getDimensions()[1] &&
             "Their second dimension must both be K");

      // Multiply constant ratio to B and C, be aware if alpha, beta are floating points and tensor type is integer
      internal::visitTensorWithFunc<PP_UNWRAP(NUMERIC_TENSORTYPE_LIST)>(
          inputBTensor, internal::MultiplyTensorByRatio(pG->getAlpha()));
      internal::visitTensorWithFunc<PP_UNWRAP(NUMERIC_TENSORTYPE_LIST)>(
          inputCTensor, internal::MultiplyTensorByRatio(pG->getBeta()));

      // Tensor A: (1, K), Tensor B: (M, K) swim:(N, K)

      // Get original Tensor A': (1, C, D0, D1, ...) swim:(N, C, D0, D1, ...)
      Tensor* inputOriginalA = getTensorBeforeShaping(pCG, *inputATensor);
      bool bEraseNodes = (inputOriginalA != inputATensor);

      if (inputOriginalA->getNumOfDimensions() != 4) {
        inputOriginalA = addReshapeBefore(pCG, inputOriginalA);

        assert(inputOriginalA->getNumOfDimensions() == 4 &&
               "Now only support original num of dimensions == 4");
      }

      // Erase nodes between A and A'
      if (bEraseNodes) {
        eraseNodesBetween(pCG, inputOriginalA, inputATensor);
      }

      // Construct Conv using A', B, and C, and Reshape to 2D
      // The shape must be the same as original Gemm's output
      ComputeOperator* pConv = constructConvAndReshape(pCG, inputOriginalA, inputBTensor, inputCTensor);
      
      Value* outValue = node.getOutput(0);
      outValue->clearDefine();
      pConv->addOutput(*outValue);

      // Do not removeAllOutputs here, because the output Value
      // has been clear define
      node.removeAllInputs();
      rmList.emplace_back(&node);
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
