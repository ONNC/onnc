//===- MaxRoiPoolLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/MaxRoiPoolLower.h>
#include <onnc/IR/Compute/MaxRoiPool.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// MaxRoiPoolLower
//===----------------------------------------------------------------------===//
MaxRoiPoolLower::MaxRoiPoolLower()
{
}

MaxRoiPoolLower::~MaxRoiPoolLower()
{
}

int MaxRoiPoolLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("MaxRoiPool"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
MaxRoiPoolLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() != 2)
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
  if (!pNode.hasAttribute(::onnx::Symbol("pooled_shape")))
    return nullptr;

  // create operators
  onnc::MaxRoiPool* op = pGraph.addOperator<onnc::MaxRoiPool>(
    pNode.is(::onnx::Symbol("pooled_shape")));

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("spatial_scale")))
    op->setSpatialScale(pNode.f(::onnx::Symbol("spatial_scale")));

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
