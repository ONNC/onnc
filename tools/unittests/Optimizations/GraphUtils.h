//===- GraphUtils.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_OPT_TEST_GRAPH_UTILS
#define ONNC_OPT_TEST_GRAPH_UTILS
#include <onnc/ADT/StringList.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/ComputeGraph.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Create Compute Graph Helper
//===----------------------------------------------------------------------===//
template<typename TensorTy>
Tensor* CreateComputeTensor(ComputeGraph& pCG, const StringRef& pName,
                            const Tensor::Dimensions& pDims)
{
  Tensor* t = pCG.addValue<TensorTy>(pName);
  t->setDimensions(pDims);
  return t;
}

Tensor*
CreateFloatComputeTensor(ComputeGraph& pCG, const StringRef& pName,
                         const Tensor::Dimensions& pDims)
{
  return CreateComputeTensor<FloatTensor>(pCG, pName, pDims);
}

template<typename OpTy, typename ... NodeCtorParams>
OpTy* CreateComputeOperator(ComputeGraph& pCG,
                            const StringList& pInputNames,
                            NodeCtorParams&& ... pParams)
{
  OpTy* op = pCG.addOperator<OpTy>(std::forward<NodeCtorParams>(pParams)...);
  for (auto& iname : pInputNames)
    op->addInput(*pCG.getValue<Tensor>(iname));
  return op;
}

template <typename TensorTy>
void CreateWeightOperator(ComputeGraph &pCG, const std::string &pName,
                          const Tensor::Dimensions &pDims) {
  Initializer *init = pCG.addOperator<Initializer>(pName);
  Tensor *value = CreateComputeTensor<TensorTy>(pCG, pName, pDims);
  init->setTensor(*value);
}

void CreateFloatWeightOperator(ComputeGraph &pCG,
                               const std::string &pName,
                               const Tensor::Dimensions &pDims) {
  CreateWeightOperator<FloatTensor>(pCG, pName, pDims);
}

template <typename TensorTy>
void CreateWeightOperatorWithValues(ComputeGraph &pCG, const std::string &pName,
                                    const Tensor::Dimensions &pDims,
                                    const typename TensorTy::ValueList& values) {
  Initializer *init = pCG.addOperator<Initializer>(pName);
  TensorTy *value = pCG.addValue<TensorTy>(pName);
  value->setDimensions(pDims);
  value->getValues() = values;
  init->setTensor(*value);
}

void CreateFloatWeightOperatorWithValues(ComputeGraph &pCG,
                                         const std::string &pName,
                                         const Tensor::Dimensions &pDims,
                                         const FloatTensor::ValueList& values) {
  CreateWeightOperatorWithValues<FloatTensor>(pCG, pName, pDims, values);
}

//===----------------------------------------------------------------------===//
// Second Layer Helper
//===----------------------------------------------------------------------===//
IntsAttr::VectorType GetInts(const IntsAttr::VectorType& pVec)
{
  return pVec;
}

ComputeGraph& BuildGraph(Module& pM, const StringRef& name)
{
  IRBuilder builder(pM);
  return *builder.CreateComputeGraph(name);
}

template <typename TensorTy = FloatTensor>
void AddInput(ComputeGraph &cg, const StringRef &name,
              const Tensor::Dimensions &pDims) {
  cg.addOperator<InputOperator>()->setTensor(
      *CreateComputeTensor<TensorTy>(cg, name, pDims));
}

template <typename OpTy, typename TensorTy = FloatTensor, typename... NodeCtorParams>
void AddOperator(ComputeGraph &cg,
                 const StringList &pInputNames,
                 const StringRef &outputName,
                 const Tensor::Dimensions &outputDims,
                 NodeCtorParams &&... pParams) {
  CreateComputeOperator<OpTy>(cg, pInputNames, std::forward<NodeCtorParams>(pParams)...)
    ->addOutput(*CreateComputeTensor<TensorTy>(cg, outputName, outputDims));
}

template <typename OpTy, typename TensorTy = FloatTensor, typename... NodeCtorParams>
void AddOperator(ComputeGraph &cg,
                 const StringList &pInputNames,
                 const StringList &pOutputNames,
                 const std::vector<Tensor::Dimensions> &pOutputDims,
                 NodeCtorParams &&... pParams) {
  auto pNode = CreateComputeOperator<OpTy>(cg, pInputNames, std::forward<NodeCtorParams>(pParams)...);
  assert(pOutputNames.size() == pOutputDims.size());
  for (StringList::size_type idx = 0; idx < pOutputNames.size(); ++idx) {
    pNode->addOutput(*CreateComputeTensor<TensorTy>(cg, pOutputNames[idx], pOutputDims[idx]));
  }
}

void AddOutput(ComputeGraph &cg, const StringList &pOutputNames) {
  CreateComputeOperator<OutputOperator>(cg, pOutputNames);
}

#endif // ONNC_OPT_TEST_GRAPH_UTILS
