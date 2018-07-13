//===- ReluLower.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/ReluLower.h>
#include <onnc/IR/Compute/Relu.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReluLower
//===----------------------------------------------------------------------===//
ReluLower::ReluLower()
{
}

ReluLower::~ReluLower()
{
}

int ReluLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("Relu"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
ReluLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
  ::onnx::Value* ox = pNode.inputs()[0];
  if (!ox->has_unique_name())
    return nullptr;

  ::onnx::Value* oy = pNode.outputs()[0];
  if (!oy->has_unique_name())
    return nullptr;

  onnc::Relu* op = pGraph.addOperator<onnc::Relu>();

  op->setX(*pGraph.getValue<onnc::Tensor>(ox->uniqueName()));
  op->setY(*pGraph.getValue<onnc::Tensor>(oy->uniqueName()));
  return op;
}
