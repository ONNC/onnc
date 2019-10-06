//===- DivideGlobalAPIntoAPs.cpp ------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/AveragePool.h>
#include <onnc/IR/Compute/GlobalAveragePool.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/Mul.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Transforms/Optimizations/OptimizationsUtils.h>
#include <onnc/Transforms/Optimizations/DivideGlobalAPIntoAPs.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// DivideGlobalAPIntoAPs
//===----------------------------------------------------------------------===//
unsigned DivideGlobalAPIntoAPs::tensorIdx = 0;

DivideGlobalAPIntoAPs::ValueType
DivideGlobalAPIntoAPs::getBestSize(const ValueType& kernelSize,
                                   const ValueType& maxKernelSize) {
  assert(kernelSize < int64_t(1000000));

  ValueType cnt = 1;
  ValueType curValue = maxKernelSize;

  while (curValue < kernelSize) {
    curValue *= maxKernelSize;
    ++cnt;
  }

  return cnt;
}

DivideGlobalAPIntoAPs::VectorType
DivideGlobalAPIntoAPs::canBeComposedOf(const ValueType& kernelSize,
                                       const ValueType& maxKernelSize) {
  VectorType retVector;

  assert(maxKernelSize >= 2 && "maxKernelSize must greater equal than 2");

  if (kernelSize <= maxKernelSize) {
    retVector.emplace_back(kernelSize);
    return retVector;
  }

  ValueType curSize = kernelSize;
  for(ValueType sz = maxKernelSize; sz >= 2; --sz) {
    while((curSize % sz) == 0) {
      retVector.emplace_back(sz);
      curSize /= sz;
    }
  }

  if (curSize != 1 || retVector.size() > getBestSize(kernelSize, maxKernelSize)) {
    retVector.clear();
  }

  return retVector;
}

DivideGlobalAPIntoAPs::VectorType
DivideGlobalAPIntoAPs::divideKernelSizeOf(const ValueType& kernelSize,
                                          const ValueType& maxKernelSize)
{
  VectorType lstOfKernels;
  ValueType start = kernelSize;
  do {
      lstOfKernels = canBeComposedOf(start, maxKernelSize);
      ++start;
  }
  while(lstOfKernels.empty());

  return lstOfKernels;
}

template <int T>
std::pair<ComputeOperator*, ComputeOperator*>
DivideGlobalAPIntoAPs::genListOfAPsMul(ComputeGraph& pCG,
                                       const Tensor* inputTensor,
                                       const VectorType& lstOfKernels)
{
  assert(false && "should not reach here, no matched tensor type");
  return {nullptr, nullptr};
}

template <typename FirstTensorType, typename... RestTensorTypes, int T>
std::pair<ComputeOperator*, ComputeOperator*>
DivideGlobalAPIntoAPs::genListOfAPsMul(ComputeGraph& pCG,
                                       const Tensor* inputTensor,
                                       const VectorType& lstOfKernels)
{
  static const std::string prefixName = "divide_globalap_into_aps_";

  const auto* pT = dynamic_cast<const FirstTensorType*>(inputTensor);
  if (pT == nullptr) {
    return genListOfAPsMul<RestTensorTypes...>(pCG, inputTensor, lstOfKernels);
  }
  else {
    // Assume inputTensor Dims : (N, C, H, W)
    // lstOfKernels(according to max(H, W)): (k1, k2, ..., kt)
    assert(inputTensor->getNumOfDimensions() == 4);
    assert(lstOfKernels.size() >= 2);

    auto createTensor = [&pCG] () {
      FirstTensorType* outTensor =
        pCG.addValue<FirstTensorType>(prefixName + std::to_string(tensorIdx));
      ++tensorIdx;
      assert(outTensor != nullptr && "The name must be unique");
      return outTensor;
    };

    std::pair<ComputeOperator*, ComputeOperator*> retPair;
    VectorType::size_type n = lstOfKernels.size();
    FirstTensorType* preTensor = nullptr;
    FirstTensorType* outTensor = nullptr;
    ValueType inputHW = inputTensor->getDimensions()[2] *
                        inputTensor->getDimensions()[3];
    ValueType totalHW = 1;

    // create AveragePools
    for (VectorType::size_type idx = 0; idx < n; ++idx) {
      const Tensor::Dimensions& lastDims = (preTensor == nullptr) ? inputTensor->getDimensions() :
                                                                    preTensor->getDimensions();
      const ValueType H = lastDims[2];
      const ValueType W = lastDims[3];

      const ValueType hSize = lstOfKernels[idx] < H ? lstOfKernels[idx] : H;
      const ValueType wSize = lstOfKernels[idx] < W ? lstOfKernels[idx] : W;
      totalHW *= hSize * wSize;

      if (idx == n-1) {
        assert(lastDims[2] <= hSize && lastDims[3] <= wSize &&
               "The last kernel must cover the tensor");
      }

      IntsAttr pKernel(IntsAttr::VectorType(2, 0));
      pKernel.at(0) = hSize;
      pKernel.at(1) = wSize;

      IntsAttr pPads(IntsAttr::VectorType(4, 0));
      pPads.at(0) = (hSize - (H % hSize)) % hSize;
      pPads.at(1) = (wSize - (W % wSize)) % wSize;

      // strides is the same as kernelShape
      AveragePool *pA = pCG.addOperator<AveragePool>(StringAttr("NOTSET"), 1,
                                                     pKernel, pPads, pKernel);
      if (preTensor != nullptr) {
        pA->addInput(*preTensor);
      }

      // If it's not last one or it has padding, then we need to add Tensor
      if (idx != n-1 || inputHW != totalHW) {
        // K -> (K + (sz - 1)) / sz
        Tensor::Dimensions newDims = lastDims;
        newDims[2] = (newDims[2] + hSize-1) / hSize;
        newDims[3] = (newDims[3] + wSize-1) / wSize;

        outTensor = createTensor();
        outTensor->setDimensions(newDims);
        pA->addOutput(*outTensor);
        preTensor = outTensor;
      }

      if (idx == 0) retPair.first = static_cast<ComputeOperator*>(pA);
      if (idx == n-1) retPair.second = static_cast<ComputeOperator*>(pA);
    }

    // check whether there's a need of Mul
    if (inputHW != totalHW) {
      Initializer* pIRatio = pCG.addOperator<Initializer>();
      FirstTensorType* ratioTensor = createTensor();

      using RatioType = typename FirstTensorType::ValueType;
      RatioType HW = static_cast<RatioType>(inputHW);
      RatioType curHW = static_cast<RatioType>(totalHW);
      RatioType ratio = curHW / HW;

      ratioTensor->setDimensions({1});
      ratioTensor->getValues() = typename FirstTensorType::ValueList(1, ratio);

      pIRatio->addOutput(*ratioTensor);

      Mul *pM = pCG.addOperator<Mul>();
      assert(preTensor != nullptr);
      pM->addInput(*preTensor);
      pM->addInput(*ratioTensor);
      retPair.second = static_cast<ComputeOperator*>(pM);
    }
    return retPair;
  }
  return {nullptr, nullptr};
}

Pass::ReturnType DivideGlobalAPIntoAPs::runOnModule(Module& pModule)
{
  Pass::ReturnType ret = BaseType::runOnModule(pModule);

  if (ret != kModuleNoChanged) {
    pModule.eraseUnusedValues();
  }

  return ret;
}

Pass::ReturnType DivideGlobalAPIntoAPs::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType ret = Pass::kModuleNoChanged;

  // Start from rear and stand on GlobalAveragePool, run in reversed order
  // Will not repeat on new AveragePool or Mul that had been created
  ComputeOperator* node;
  bool erased;
  for (pCG.getRear(node); node != nullptr;
       node = erased ? node : node->getPrevNode()) {
    erased = false;

    if (GlobalAveragePool* pG = dyn_cast<GlobalAveragePool>(node)) {
      assert(pG->getNumOfInputs() == 1 &&
             "GlobalAveragePool must have exactly one input");
      assert(pG->getNumOfOutputs() == 1 &&
             "GlobalAveragePool must have exactly one output");

      Tensor* inputTensor = pG->getInput(0);
      assert(inputTensor->getNumOfDimensions() == 4 &&
             "Currently only support input tensor of four dimensions, This \
assertion should be removed after backend can support generalized \
version of AvergePool, i.e. support 3 or 5 dims ... etc.");

      if (inputTensor->getDimensions()[2] <= m_MaxKernelSize &&
          inputTensor->getDimensions()[3] <= m_MaxKernelSize) {
        // Transform GlobalAveragePool into one AveragePool
        ValueType ndims = static_cast<ValueType>(inputTensor->getNumOfDimensions());
        assert(ndims > 2);
        ndims -= 2;

        using IntsSize = IntsAttr::VectorType::size_type;
        IntsAttr pPads(IntsAttr::VectorType(static_cast<IntsSize>(2 * ndims), 0));
        IntsAttr pKernel(IntsAttr::VectorType(static_cast<IntsSize>(ndims)));
        for (ValueType idx = 0; idx < ndims; ++idx) {
          pKernel.at(idx) = inputTensor->getDimensions()[idx+2];
        }
        AveragePool *pA = pCG.addOperator<AveragePool>(StringAttr("NOTSET"), 0,
                                                       pKernel, pPads, pKernel);

        pA->addInput(*inputTensor);

        Value* outV = node->getOutput(0);
        outV->clearDefine();
        pA->addOutput(*outV);
      }
      else {
        // Transform GlobalAveragePool into several AveragePools + (Mul)
        assert(inputTensor->getNumOfDimensions() == 4 &&
               "Currently only support input tensor of four dimensions");

        ValueType K = std::max(inputTensor->getDimensions()[2],
                               inputTensor->getDimensions()[3]);
        VectorType lstOfKernels = divideKernelSizeOf(K, m_MaxKernelSize);

        // the last element of lstOfKernels is the size of padding
        assert(lstOfKernels.size() >= 2 &&
               "There must be at least two AveragePools");

        auto pairOfNodePtrs = genListOfAPsMul<PP_UNWRAP(FP_TENSORTYPE_LIST)>
                                (pCG, inputTensor, lstOfKernels);

        pairOfNodePtrs.first->addInput(*inputTensor);
        
        Value* outV = node->getOutput(0);
        outV->clearDefine();
        pairOfNodePtrs.second->addOutput(*outV);
      }

      ComputeOperator* rmNode = node;
      node = node->getPrevNode();
      // Do not removeAllOutputs() here, because the output has been clearDefine()
      rmNode->removeAllInputs();
      pCG.erase(*rmNode);

      erased = true;
      ret |= Pass::kModuleChanged;
    }
  }

  if (ret != kModuleNoChanged) {
    pCG.topologicalSort();
  }

  return ret;
}
