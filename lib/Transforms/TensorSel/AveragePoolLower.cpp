//===- AveragePoolLower.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/AveragePoolLower.h>
#include <onnc/IR/Compute/AveragePool.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// AveragePoolLower
//===----------------------------------------------------------------------===//
AveragePoolLower::AveragePoolLower()
{
}

AveragePoolLower::~AveragePoolLower()
{
}

int AveragePoolLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("AveragePool"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
AveragePoolLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
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

  // check required attributes
  if (!pNode.hasAttribute(::onnx::Symbol("pads")))
    return nullptr;

  // create operators
  onnc::AveragePool* op = pGraph.addOperator<onnc::AveragePool>(
      pNode.is(::onnx::Symbol("kernel_shape")));

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("auto_pad")))
    op->setAutoPad(pNode.s(::onnx::Symbol("auto_pad")));
  if (pNode.hasAttribute(::onnx::Symbol("count_include_pad")))
    op->setCountIncludePad(pNode.i(::onnx::Symbol("count_include_pad")));
  if (pNode.hasAttribute(::onnx::Symbol("pads")));
    op->setPads(pNode.is(::onnx::Symbol("pads")));
  if (pNode.hasAttribute(::onnx::Symbol("strides")));
    op->setStrides(pNode.is(::onnx::Symbol("strides")));

  // set input/output
  onnc::Tensor* x = pGraph.getValue<onnc::Tensor>(
      pNode.inputs()[onnc::AveragePool::kX]->uniqueName());
  onnc::Tensor* y = pGraph.getValue<onnc::Tensor>(
      pNode.outputs()[onnc::AveragePool::kY]->uniqueName());

  op->setX(*x);
  op->setY(*y);
  return op;
}
