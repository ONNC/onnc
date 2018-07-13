//===- ReshapeLower.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/ReshapeLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReshapeLower
//===----------------------------------------------------------------------===//
ReshapeLower::ReshapeLower()
{
}

ReshapeLower::~ReshapeLower()
{
}

int ReshapeLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("Reshape"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
ReshapeLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
