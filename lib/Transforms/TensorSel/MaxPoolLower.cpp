//===- MaxPoolLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/MaxPoolLower.h>
#include <onnc/IR/Compute/MaxPool.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// MaxPoolLower
//===----------------------------------------------------------------------===//
MaxPoolLower::MaxPoolLower()
{
}

MaxPoolLower::~MaxPoolLower()
{
}

int MaxPoolLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("MaxPool"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
MaxPoolLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
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
  if (!pNode.hasAttribute(::onnx::Symbol("kernel_shape")))
    return nullptr;

  // create operators
  onnc::MaxPool* op = pGraph.addOperator<onnc::MaxPool>(
    pNode.is(::onnx::Symbol("kernel_shape")));

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("auto_pad")))
    op->setAutoPad(pNode.s(::onnx::Symbol("auto_pad")));
  if (pNode.hasAttribute(::onnx::Symbol("pads")))
    op->setPads(pNode.is(::onnx::Symbol("pads")));
  if (pNode.hasAttribute(::onnx::Symbol("strides")))
    op->setStrides(pNode.is(::onnx::Symbol("strides")));

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
