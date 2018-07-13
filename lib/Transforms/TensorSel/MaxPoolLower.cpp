//===- MaxPoolLower.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/MaxPoolLower.h>
#include <onnc/IR/Compute/MaxPool.h>

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
  // check input/output name
  if (1 != pNode.inputs().size())
    return nullptr;

  for (::onnx::Value* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  if (1 != pNode.outputs().size())
    return nullptr;

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

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("auto_pad")))
    op->setAutoPad(pNode.s(::onnx::Symbol("auto_pad")));
  if (pNode.hasAttribute(::onnx::Symbol("pads")))
    op->setPads(pNode.is(::onnx::Symbol("pads")));
  if (pNode.hasAttribute(::onnx::Symbol("strides")))
    op->setStrides(pNode.is(::onnx::Symbol("strides")));

  // set input/output
  for (::onnx::Value* xv : pNode.inputs())
    op->addInput(*pGraph.getValue<onnc::Tensor>(xv->uniqueName()));

  for (::onnx::Value* xv : pNode.outputs())
    op->addOutput(*pGraph.getValue<onnc::Tensor>(xv->uniqueName()));

  return op;
}
