//===- MeanVarianceNormalizationLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/MeanVarianceNormalizationLower.h>
#include <onnc/IR/Compute/MeanVarianceNormalization.h>
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// MeanVarianceNormalizationLower
//===----------------------------------------------------------------------===//
MeanVarianceNormalizationLower::MeanVarianceNormalizationLower()
{
}

MeanVarianceNormalizationLower::~MeanVarianceNormalizationLower()
{
}

int MeanVarianceNormalizationLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("MeanVarianceNormalization"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
MeanVarianceNormalizationLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() != 1)
    return nullptr;

  if (pNode.outputs().size() != 1)
    return nullptr;

  // check input/output name
  for (::onnx::Value* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  for (::onnx::Value* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // check default attributes
  

  // create operators
  onnc::MeanVarianceNormalization* op = pGraph.addOperator<onnc::MeanVarianceNormalization>();

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("across_channels")))
    op->setAcrossChannels(pNode.i(::onnx::Symbol("across_channels")));
  if (pNode.hasAttribute(::onnx::Symbol("normalize_variance")))
    op->setNormalizeVariance(pNode.i(::onnx::Symbol("normalize_variance")));

  // set input/output
  for (::onnx::Value* xv : pNode.inputs()) {
    onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addInput(*tensor);
  }

  for (::onnx::Value* xv : pNode.outputs()) {
    onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addOutput(*tensor);
  }
  return op;
}
