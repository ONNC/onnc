//===- LRNLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/LRNLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LRNLower
//===----------------------------------------------------------------------===//
LRNLower::LRNLower()
{
}

LRNLower::~LRNLower()
{
}

int LRNLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("LRN"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
LRNLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
