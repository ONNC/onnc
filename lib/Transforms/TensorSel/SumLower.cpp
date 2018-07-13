//===- SumLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/SumLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// SumLower
//===----------------------------------------------------------------------===//
SumLower::SumLower()
{
}

SumLower::~SumLower()
{
}

int SumLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("Sum"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
SumLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
