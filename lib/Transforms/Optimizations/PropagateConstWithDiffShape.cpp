//===- PropagateConstWithDiffShape.cpp ------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/Squeeze.h>
#include <onnc/IR/Compute/Unsqueeze.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Flatten.h>
#include <onnc/Transforms/Optimizations/PropagateConstWithDiffShape.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// PropagateConstWithDiffShape
//===----------------------------------------------------------------------===//
const static std::unordered_set<const void *>
  shapingNodeIDs {&Flatten::ID, &Reshape::ID, &Squeeze::ID, &Unsqueeze::ID};

static bool validate(const Squeeze* const pS)
{
  assert(pS->getNumOfInputs() == 1 &&
      "Squeeze must have exactly one input");
  assert(pS->getNumOfOutputs() == 1 &&
      "Squeeze must have exactly one output");

  const Tensor* const inputTensor = pS->getData();
  const Tensor* const outputTensor = pS->getSqueezed();
  Tensor::Dimensions copyInputDims = inputTensor->getDimensions();
  const Tensor::Dimensions& outputDims = outputTensor->getDimensions();
  IntsAttr::VectorType copyAxes = pS->getAxes().vector();
  std::sort(begin(copyAxes), end(copyAxes), std::greater<int64_t>());
  for (const auto& axis : copyAxes) {
    assert(axis >= 0 && axis < copyInputDims.size() && copyInputDims[axis] == 1);
    copyInputDims.erase(next(begin(copyInputDims), axis));
  }
  assert(copyInputDims == outputDims && "Dimensions must be the same after Squeeze");
  return true;
}

static bool validate(const Unsqueeze* const pU)
{
  assert(pU->getNumOfInputs() == 1 &&
      "Unsqueeze must have exactly one input");
  assert(pU->getNumOfOutputs() == 1 &&
      "Unsqueeze must have exactly one output");

  const Tensor* const inputTensor = pU->getData();
  const Tensor* const outputTensor = pU->getExpanded();
  const Tensor::Dimensions& inputDims = inputTensor->getDimensions();
  Tensor::Dimensions copyOutputDims = outputTensor->getDimensions();
  IntsAttr::VectorType copyAxes = pU->getAxes().vector();
  std::sort(begin(copyAxes), end(copyAxes), std::greater<IntsAttr::VectorType::value_type>());
  for (const auto& axis : copyAxes) {
    assert(axis >= 0 && axis < copyOutputDims.size() && copyOutputDims[axis] == 1);
    copyOutputDims.erase(next(begin(copyOutputDims), axis));
  }
  assert(inputDims == copyOutputDims && "Dimensions must be the same after Unsqueeze");
  return true;
}

static bool validate(const Reshape* const pR)
{
  assert(pR->getNumOfInputs() == 2 &&
      "Reshape must have exactly two inputs");
  assert(pR->getNumOfOutputs() == 1 &&
      "Reshape must have exactly one output");

  const Tensor* const inputTensor = pR->getData();
  const Tensor* const shapeTensor = pR->getShape();
  const Tensor* const outputTensor = pR->getReshaped();

  const ComputeOperator* const shapeNode = static_cast<const ComputeOperator*>(shapeTensor->getDefine());
  if (!isa<Initializer>(shapeNode)) {
    // Do not support if the input shape is not Initializer
    assert(0 && "Now only support input shape is Initializer, and this \
assertion is only used to check this condition. This assertion should \
be removed for general usage (just return and do nothing).");
    return false;
  }

  const Tensor::Dimensions& inputDims = inputTensor->getDimensions();
  const Tensor::Dimensions& outputDims = outputTensor->getDimensions();

  const Int64Tensor* const intShapeTensor = dynamic_cast<const Int64Tensor*>(shapeTensor);
  assert(intShapeTensor != nullptr && "Shape tensor must be type of Int64Tensor");

  assert(intShapeTensor->getValues().size() != 0 &&
      intShapeTensor->getValues().size() == outputDims.size());

  Tensor::Dimension oriTot = 1, resTot = 1;
  for (const auto& dim : inputDims) {
    oriTot *= dim;
  }
  for (const auto& dim : outputDims) {
    resTot *= dim;
  }
  assert(oriTot == resTot && "The total size of shape should be the same");
  return true;
}

static bool validate(const Flatten* const pF)
{
  assert(pF->getNumOfInputs() == 1 &&
      "Flatten must have exactly one input");
  assert(pF->getNumOfOutputs() == 1 &&
      "Flatten must have exactly one output");

  const Tensor* const inputTensor = pF->getInput();
  const Tensor* const outputTensor = pF->getOutput();
  const Tensor::Dimensions& inputDims = inputTensor->getDimensions();
  const Tensor::Dimensions& outputDims = outputTensor->getDimensions();

  IntAttr axis = pF->getAxis();
  assert(axis.value() >= 0 && axis.value() <= inputDims.size());

  Tensor::Dimension fDim = 1, sDim = 1;
  for (Tensor::Size i = 0; i < axis.value(); ++i) { fDim *= inputDims[i]; }
  for (Tensor::Size i = axis.value(); i < inputDims.size(); ++i) { sDim *= inputDims[i]; }
  assert(outputDims.size() == 2 && outputDims[0] == fDim && outputDims[1] == sDim);
  return true;
}

template <typename ShapingNode>
static void removeShapingNode(ShapingNode* const pS,
                              Initializer* pI,
                              ComputeGraph& pCG,
                              bool& erased)
{
  // check the input Value and output Value size are
  // correct under different shaping node.
  // now only support input shape to Reshape is Initializer (constant)
  if (!validate(pS)) return;

  // if the num of uses of pI > 1, clone one Initializer
  if (pI->getOutput<Tensor>()->getUses().size() > 1) {
    // First clone the original Tensor
    // Need the same dimensions and values
    Tensor* clonedTensor = pI->getOutput<Tensor>()->clone();
    clonedTensor = pCG.addValue<Tensor>(clonedTensor);
    assert(clonedTensor != nullptr && "Cloned tensor name must be unique");

    // Add a new Initializer here
    pI = pCG.addOperator<Initializer>(pI->name().str() + "<clone>");
    assert(pI != nullptr && "The name must be unique");

    // Set the cloned tensor
    pI->setTensor(*clonedTensor);
  }

  Tensor* const constTensor = pI->getOutput<Tensor>();
  Tensor* const outTensor = pS->getOutput(0);
  constTensor->setDimensions(outTensor->getDimensions());
  outTensor->replaceAllUsesWith(*constTensor);

  // If pS is Reshape, the second input might become
  // a dangling node.
  // Currently only remove additional input up to one layer
  // Complete version should do the same as in EliminateDeadEnd pass
  if (pS->getNumOfInputs() >= 2) {
    for (unsigned int idx = 1; idx < pS->getNumOfInputs(); ++idx) {
      Value* inputValue = pS->getInput(idx);
      if (inputValue->getUses().size() == 1) {
        ComputeOperator* uselessNode = static_cast<ComputeOperator*>(inputValue->getDefine());
        uselessNode->removeAllInputs();
        uselessNode->removeAllOutputs();
        pCG.erase(*uselessNode);
      }
    }
  }

  pS->removeAllInputs();
  pS->removeAllOutputs();
  erased = true;
}

template <int T = 0>
static void visitShapingNode(ComputeOperator& node,
                             Initializer* pI,
                             ComputeGraph& pCG,
                             bool& erased)
{
  assert(false && "should not reach here, no matched node type");
}

template <typename FirstNodeType, typename... RestNodeTypes, int T = 0>
static void visitShapingNode(ComputeOperator& node,
                             Initializer* pI,
                             ComputeGraph& pCG,
                             bool& erased)
{
  if (node.getID() == &FirstNodeType::ID) {
    removeShapingNode<FirstNodeType>(dyn_cast<FirstNodeType>(&node), pI, pCG, erased);
  } else {
    visitShapingNode<RestNodeTypes...>(node, pI, pCG, erased);
  }
  return;
}

Pass::ReturnType PropagateConstWithDiffShape::runOnModule(Module& pModule)
{
  const Pass::ReturnType ret = BaseType::runOnModule(pModule);

  if (ret != kModuleNoChanged) {
    pModule.eraseUnusedValues();
  }

  return ret;
}

Pass::ReturnType PropagateConstWithDiffShape::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType ret = Pass::kModuleNoChanged;

  // Can deal with Initializer -> Unsqueeze -> Reshape -> Flatten case.
  std::vector<ComputeOperator*> rmList;
  for (ComputeOperator &node : pCG) {
    if (shapingNodeIDs.count(node.getID())) {
      assert(node.getNumOfInputs() >= 1 &&
             "Shaping Node must have no less than one input");
      assert(node.getNumOfOutputs() == 1 &&
             "Shaping Node must have exactly one output");

      // Assume input data always at index 0
      Value* inputValue = node.getInput(0);
      ComputeOperator* inputNode = static_cast<ComputeOperator*>(inputValue->getDefine());
      if (Initializer* pI = dyn_cast<Initializer>(inputNode)) {
        bool erased = false;

        // This function will modify pCG and update erased.
        visitShapingNode<Squeeze, Unsqueeze, Reshape, Flatten>(node, pI, pCG, erased);

        if (erased) rmList.emplace_back(&node);
      }
      else if (node.getID() != &Reshape::ID) {
        // This else if block can be removed
        // It's only used to check current supporting conditions.
        // Need to be removed in general cases.
        assert(0 && "The shaping node is not Reshape, so the input must be Initializer for now");
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
