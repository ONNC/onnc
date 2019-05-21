//===- ComputeGraphTest.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/StringList.h>
#include <onnc/IR/IRBuilder.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Gemm.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/LRN.h>
#include <onnc/IR/Compute/MaxPool.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Softmax.h>
#include <onnc/IR/Compute/ATen.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Support/OStrStream.h>
#include <skypat/skypat.h>
#include <onnc/CodeGen/BuildMemOperand.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Create Compute Graph Helper
//===----------------------------------------------------------------------===//
template<typename TensorTy>
static Tensor* CreateComputeTensor(ComputeGraph& pCG, const StringRef& pName,
                                   const Tensor::Dimensions& pDims)
{
  Tensor* t = pCG.addValue<TensorTy>(pName);
  t->setDimensions(pDims);
  return t;
}

static Tensor*
CreateFloatComputeTensor(ComputeGraph& pCG, const StringRef& pName,
                         const Tensor::Dimensions& pDims)
{
  return CreateComputeTensor<FloatTensor>(pCG, pName, pDims);
}

template<typename TensorTy>
static void CreateWeightOperator(ComputeGraph& pCG, const std::string& pName,
                                 const Tensor::Dimensions& pDims)
{
  Initializer* init = pCG.addOperator<Initializer>(pName);
  Tensor* value = CreateComputeTensor<TensorTy>(pCG, pName, pDims);
  init->setTensor(*value);
}

static void
CreateFloatWeightOperator(ComputeGraph& pCG, const std::string& pName,
                          const Tensor::Dimensions& pDims)
{
  CreateWeightOperator<FloatTensor>(pCG, pName, pDims);
}

template<typename OpTy, typename ... NodeCtorParams>
static OpTy* CreateComputeOperator(ComputeGraph& pCG,
                                   const StringList& pInputNames,
                                   NodeCtorParams&& ... pParams)
{
  OpTy* op = pCG.addOperator<OpTy>(pParams...);
  for (auto& iname : pInputNames)
    op->addInput(*pCG.getValue<Tensor>(iname));
  return op;
}

template<typename VTy>
static std::vector<VTy> GetValues(const std::vector<VTy>& pVec)
{
  return pVec;
}

//===----------------------------------------------------------------------===//
// Create Model
//===----------------------------------------------------------------------===//
static void CreateAlexNet(Module& pM)
{
  IRBuilder builder(pM);

  ComputeGraph& cg = *builder.CreateComputeGraph("Alexnet");

  // Create Input.
  cg.addOperator<InputOperator>()->setTensor(
    *CreateFloatComputeTensor(cg, "data_0", {10, 3, 277, 277}));

  // Create weights.
  CreateFloatWeightOperator(cg, "conv1_w_0", {96, 3, 11, 11});
  CreateFloatWeightOperator(cg, "conv1_b_0", {96});
  CreateFloatWeightOperator(cg, "conv2_w_0", {256, 48, 5, 5});
  CreateFloatWeightOperator(cg, "conv2_b_0", {256});
  CreateFloatWeightOperator(cg, "conv3_w_0", {384, 256, 3, 3});
  CreateFloatWeightOperator(cg, "conv3_b_0", {384});
  CreateFloatWeightOperator(cg, "conv4_w_0", {384, 192, 3, 3});
  CreateFloatWeightOperator(cg, "conv4_b_0", {384});
  CreateFloatWeightOperator(cg, "conv5_w_0", {256, 192, 3, 3});
  CreateFloatWeightOperator(cg, "conv5_b_0", {256});
  CreateFloatWeightOperator(cg, "fc6_w_0", {4096, 9216});
  CreateFloatWeightOperator(cg, "fc6_b_0", {4096});
  CreateFloatWeightOperator(cg, "fc7_w_0", {4096, 4096});
  CreateFloatWeightOperator(cg, "fc7_b_0", {4096});
  CreateFloatWeightOperator(cg, "fc8_w_0", {1000, 4096});
  CreateFloatWeightOperator(cg, "fc8_b_0", {1000});
  CreateWeightOperator<Int64Tensor>(cg, "OC2_DUMMY_1", {2});

  // create nodes (layers)
  CreateComputeOperator<Conv>(cg,    {"data_0", "conv1_w_0", "conv1_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "conv1_1", {10, 96, 55, 55}));

  CreateComputeOperator<Relu>(cg,    {"conv1_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "relu1_1", {10, 96, 55, 55}));

  CreateComputeOperator<LRN>(cg,     {"relu1_1"}, /* size */ IntAttr(5))
    ->addOutput(*CreateFloatComputeTensor(cg, "norm1_1", {10, 96, 55, 55}));

  CreateComputeOperator<MaxPool>(cg, {"norm1_1"},
                                 /* krnl shape */ GetValues<int64_t>({3, 3}))
    ->addOutput(*CreateFloatComputeTensor(cg, "pool1_1", {10, 96, 27, 27}));

  CreateComputeOperator<Conv>(cg,    {"pool1_1", "conv2_w_0", "conv2_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "conv2_1", {10, 256, 27, 27}));

  CreateComputeOperator<Relu>(cg,    {"conv2_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "relu2_1", {10, 256, 27, 27}));

  CreateComputeOperator<LRN>(cg,     {"relu2_1"}, IntAttr(1))
    ->addOutput(*CreateFloatComputeTensor(cg, "norm2_1", {10, 256, 27, 27}));

  CreateComputeOperator<MaxPool>(cg, {"norm2_1"},
                                 /* krnl shape */ GetValues<int64_t>({3, 3}))
    ->addOutput(*CreateFloatComputeTensor(cg, "pool2_1", {10, 256, 13, 13}));

  CreateComputeOperator<Conv>(cg,    {"pool2_1", "conv3_w_0", "conv3_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "conv3_1", {10, 384, 13, 13}));

  CreateComputeOperator<Relu>(cg,    {"conv3_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "relu3_1", {10, 384, 13, 13}));

  CreateComputeOperator<Conv>(cg,    {"relu3_1", "conv4_w_0", "conv4_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "conv4_1", {10, 384, 13, 13}));

  CreateComputeOperator<Relu>(cg,    {"conv4_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "relu4_1", {10, 384, 13, 13}));

  CreateComputeOperator<Conv>(cg,    {"relu4_1", "conv5_w_0", "conv5_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "conv5_1", {10, 256, 13, 13}));

  CreateComputeOperator<Relu>(cg,    {"conv5_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "relu5_1", {10, 256, 13, 13}));

  CreateComputeOperator<MaxPool>(cg, {"relu5_1"},
                                 /* krnl shape */ GetValues<int64_t>({3, 3}))
    ->addOutput(*CreateFloatComputeTensor(cg, "pool5_1", {10, 256, 6, 6}));

  CreateComputeOperator<Reshape>(cg,    {"pool5_1", "OC2_DUMMY_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "OC2_DUMMY_0", {10, 9216}));

  CreateComputeOperator<Gemm>(cg,    {"OC2_DUMMY_0", "fc6_w_0", "fc6_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "fc6_1", {10, 4096}));

  CreateComputeOperator<Relu>(cg,    {"fc6_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "relu6_1", {10, 4096}));

  CreateComputeOperator<Gemm>(cg,    {"relu6_1", "fc7_w_0", "fc7_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "fc7_1", {10, 4096}));

  CreateComputeOperator<Relu>(cg,    {"fc7_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "relu7_1", {10, 4096}));

  CreateComputeOperator<Gemm>(cg,    {"relu7_1", "fc8_w_0", "fc8_b_0"})
    ->addOutput(*CreateFloatComputeTensor(cg, "fc8_1", {10, 1000}));

  CreateComputeOperator<Softmax>(cg, {"fc8_1"})
    ->addOutput(*CreateFloatComputeTensor(cg, "prob_1", {10, 1000}));

  CreateComputeOperator<OutputOperator>(cg, {"prob_1"});
}

//===----------------------------------------------------------------------===//
// Testcases
//===----------------------------------------------------------------------===//
SKYPAT_F(ComputeGraphTest, print_alexnet)
{
  Module module;
  CreateAlexNet(module);

  // Ans:
  /*
  const char* opNameList[] = {
    "InputOperator",
    "Initializer", "Initializer", "Initializer", "Initializer", "Initializer",
    "Initializer", "Initializer", "Initializer", "Initializer", "Initializer",
    "Initializer", "Initializer", "Initializer", "Initializer", "Initializer",
    "Initializer", "Initializer",
    "Conv", "Relu", "LRN", "MaxPool", "Conv", "Relu", "LRN", "MaxPool",
    "Conv", "Relu", "Conv", "Relu", "Conv", "Relu", "MaxPool", "Reshape",
    "Gemm", "Relu", "Gemm", "Relu", "Gemm", "Softmax", "OutputOperator"
  };
  */

  // build memory operands
  BuildMemOperand buildMemOpnd;
  buildMemOpnd.runOnModule(module);

  // create JSON value to print
  json::Object jsonObj;
  module.printComputeGraph(jsonObj);
}

