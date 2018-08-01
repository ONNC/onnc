//===- RandomNormalLikeLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/RandomNormalLikeLower.h>
#include <onnc/IR/Compute/RandomNormalLike.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// RandomNormalLikeLower
//===----------------------------------------------------------------------===//
RandomNormalLikeLower::RandomNormalLikeLower()
{
}

RandomNormalLikeLower::~RandomNormalLikeLower()
{
}

int RandomNormalLikeLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("RandomNormalLike"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
RandomNormalLikeLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
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
  onnc::RandomNormalLike* op = pGraph.addOperator<onnc::RandomNormalLike>();

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("dtype")))
    op->setDtype(pNode.i(::onnx::Symbol("dtype")));
  if (pNode.hasAttribute(::onnx::Symbol("mean")))
    op->setMean(pNode.f(::onnx::Symbol("mean")));
  if (pNode.hasAttribute(::onnx::Symbol("scale")))
    op->setScale(pNode.f(::onnx::Symbol("scale")));
  if (pNode.hasAttribute(::onnx::Symbol("seed")))
    op->setSeed(pNode.f(::onnx::Symbol("seed")));

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
