//===- RandomUniformLikeLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/RandomUniformLikeLower.h>
#include <onnc/IR/Compute/RandomUniformLike.h>
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// RandomUniformLikeLower
//===----------------------------------------------------------------------===//
RandomUniformLikeLower::RandomUniformLikeLower()
{
}

RandomUniformLikeLower::~RandomUniformLikeLower()
{
}

int RandomUniformLikeLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("RandomUniformLike"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
RandomUniformLikeLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
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
  onnc::RandomUniformLike* op = pGraph.addOperator<onnc::RandomUniformLike>();

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("dtype")))
    op->setDtype(pNode.i(::onnx::Symbol("dtype")));
  if (pNode.hasAttribute(::onnx::Symbol("high")))
    op->setHigh(pNode.f(::onnx::Symbol("high")));
  if (pNode.hasAttribute(::onnx::Symbol("low")))
    op->setLow(pNode.f(::onnx::Symbol("low")));
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
