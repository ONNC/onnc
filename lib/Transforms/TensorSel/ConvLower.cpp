//===- ConvLower.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/ConvLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ConvLower
//===----------------------------------------------------------------------===//
ConvLower::ConvLower()
{
}

ConvLower::~ConvLower()
{
}

int ConvLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("Conv"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
ConvLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
