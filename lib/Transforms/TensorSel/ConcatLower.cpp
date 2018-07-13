//===- ConcatLower.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/ConcatLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ConcatLower
//===----------------------------------------------------------------------===//
ConcatLower::ConcatLower()
{
}

ConcatLower::~ConcatLower()
{
}

int ConcatLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("Concat"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
ConcatLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
