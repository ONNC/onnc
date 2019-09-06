//===- ExpandBatchNormalization.cpp ---------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Add.h>
#include <onnc/IR/Compute/Mul.h>
#include <onnc/IR/Compute/BatchNormalization.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/Mul.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Transforms/Optimizations/OptimizationsUtils.h>
#include <onnc/Transforms/Optimizations/ExpandBatchNormalization.h>

namespace onnc {
namespace internal {
bool allOtherFourInputsAreInitializers(const BatchNormalization& batchNormalization)
{
  using namespace internal;
  for (auto idx :
       {BatchNormalization::kScale, BatchNormalization::kB, BatchNormalization::kInMean, BatchNormalization::kInVar}) {
    if (!isDefinedByInitializer(batchNormalization.getInput(idx))) {
      return false;
    }
  }
  return true;
}

Tensor* cloneTensorAndAssignName(ComputeGraph& pCG, const Tensor& tensorToBeCloned, const std::string& newTensorName)
{
  Tensor* pNewTensor = tensorToBeCloned.clone();
  pNewTensor->setName(newTensorName);
  pNewTensor = pCG.addValue<Tensor>(pNewTensor);
  assert(pNewTensor != nullptr && "The name must be unique");
  return pNewTensor;
}

} // namespace internal

//===----------------------------------------------------------------------===//
// ExpandBatchNormalization
//===----------------------------------------------------------------------===//
unsigned ExpandBatchNormalization::tensorIdx = 0;

void ExpandBatchNormalization::expandBNToAddAndMul(ComputeGraph& pCG, BatchNormalization& batchNormalization)
{
  Tensor* const pTensorBNScale  = batchNormalization.getScale();
  Tensor* const pTensorBNB      = batchNormalization.getB();
  Tensor* const pTensorBNInMean = batchNormalization.getInMean();
  Tensor* const pTensorBNInVar  = batchNormalization.getInVar();

  const std::string& bnOutputName = batchNormalization.getOutput(BatchNormalization::kY)->getName();

  using namespace internal;
  Initializer* pIAddend      = pCG.addOperator<Initializer>();
  std::string  addendName    = bnOutputName + "(bn_expanded_addend_" + std::to_string(tensorIdx++) + ")";
  Tensor*      pTensorAddend = cloneTensorAndAssignName(pCG, *pTensorBNB, addendName);
  pIAddend->setTensor(*pTensorAddend);

  Initializer* pIMultiplier      = pCG.addOperator<Initializer>();
  std::string  multiplierName    = bnOutputName + "(bn_expanded_multiplier_" + std::to_string(tensorIdx++) + ")";
  Tensor*      pTensorMultiplier = cloneTensorAndAssignName(pCG, *pTensorBNScale, multiplierName);
  pIMultiplier->setTensor(*pTensorMultiplier);

  // squareRoot = std::sqrt(variance + epsilon);
  // addend     = (bias * squareRoot / scale - mean);
  // multiplier = (scale / squareRoot);

  // clang-format off
  Tensor* pTensorSquareRoot = pTensorBNInVar->clone();
  visitTensorWithFunc<PP_UNWRAP(NUMERIC_TENSORTYPE_LIST)>(pTensorSquareRoot, AddConstToTensor(batchNormalization.getEpsilon()));
  visitTensorWithFunc<PP_UNWRAP(NUMERIC_TENSORTYPE_LIST)>(pTensorSquareRoot, ApplyUnaryOpOnTensor(OPTYPE_SQRT));

  visitTensorWithFunc<PP_UNWRAP(NUMERIC_TENSORTYPE_LIST)>(pTensorAddend,     ApplyBinaryOpOnTensorWithSameShape(pTensorSquareRoot, OPTYPE_MUL));
  visitTensorWithFunc<PP_UNWRAP(NUMERIC_TENSORTYPE_LIST)>(pTensorAddend,     ApplyBinaryOpOnTensorWithSameShape(pTensorBNScale,    OPTYPE_DIV));
  visitTensorWithFunc<PP_UNWRAP(NUMERIC_TENSORTYPE_LIST)>(pTensorAddend,     ApplyBinaryOpOnTensorWithSameShape(pTensorBNInMean,   OPTYPE_SUB));

  visitTensorWithFunc<PP_UNWRAP(NUMERIC_TENSORTYPE_LIST)>(pTensorMultiplier, ApplyBinaryOpOnTensorWithSameShape(pTensorSquareRoot, OPTYPE_DIV));
  delete pTensorSquareRoot;
  // clang-format on

  assert(batchNormalization.getX()->getNumOfDimensions() == 4);
  assert(batchNormalization.getB()->getNumOfDimensions() >= 1);
  Tensor::Dimension M = batchNormalization.getB()->getDimensions()[0];
  pTensorAddend->setDimensions({M, 1, 1});
  pTensorMultiplier->setDimensions({M, 1, 1});

  Add* pAdd = pCG.addOperator<Add>();
  Mul* pMul = pCG.addOperator<Mul>();

  pAdd->addInput(*batchNormalization.getX());
  pAdd->addInput(*pTensorAddend);

  std::string addTensorName = "bn_add(bn_expanded_tensor_" + std::to_string(tensorIdx++) + ")";
  Tensor*     pTensorAdd    = cloneTensorAndAssignName(pCG, *batchNormalization.getY(), addTensorName);
  pAdd->addOutput(*pTensorAdd);

  pMul->addInput(*pTensorAdd);
  pMul->addInput(*pTensorMultiplier);

  batchNormalization.removeAllOutputs();
  pMul->addOutput(*batchNormalization.getY());
}

Pass::ReturnType ExpandBatchNormalization::runOnModule(Module& pModule)
{
  Pass::ReturnType    ret = Pass::kModuleNoChanged;
  Module::cg_iterator cg, cgEnd = pModule.cgEnd();
  for (cg = pModule.cgBegin(); cg != cgEnd; ++cg) {
    ret |= runOnComputeGraph(*cg->value());
  }

  if (ret != Pass::kModuleNoChanged) {
    pModule.eraseUnusedValues();
  }
  return ret;
}

Pass::ReturnType ExpandBatchNormalization::runOnComputeGraph(ComputeGraph& pCG)
{
  using namespace internal;

  std::vector<ComputeOperator*> rmList;
  for (ComputeOperator& node : pCG) {
    if (BatchNormalization* pBN = dyn_cast<BatchNormalization>(&node)) {
      assert(pBN->getNumOfInputs() == 5 && "BatchNormalization must have exactly five inputs");
      if (pBN->getNumOfOutputs() != 1) {
        continue;
      }
      if (!allOtherFourInputsAreInitializers(*pBN)) {
        continue;
      }

      // pBN->removeAllOutputs() in this function
      expandBNToAddAndMul(pCG, *pBN);

      for (auto idx : {BatchNormalization::kScale, BatchNormalization::kB, BatchNormalization::kInMean,
                       BatchNormalization::kInVar}) {
        Tensor* pBNInputTensor = pBN->getInput(idx);
        if (pBNInputTensor->getUses().size() == 1) {
          auto* pNodeToBeRemoved = static_cast<ComputeOperator*>(pBNInputTensor->getDefine());
          pNodeToBeRemoved->removeAllOutputs();
          rmList.emplace_back(pNodeToBeRemoved);
        }
      }

      pBN->removeAllInputs();
      rmList.emplace_back(&node);
    }
  }

  if (rmList.empty()) {
    return Pass::kModuleNoChanged;
  }

  for (ComputeOperator* pNode : rmList) {
    pCG.erase(*pNode);
  }

  pCG.topologicalSort();
  return Pass::kModuleChanged;
}

} // namespace onnc
