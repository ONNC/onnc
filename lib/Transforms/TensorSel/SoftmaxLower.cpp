//===- SoftmaxLower.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/SoftmaxLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// SoftmaxLower
//===----------------------------------------------------------------------===//
SoftmaxLower::SoftmaxLower()
{
}

SoftmaxLower::~SoftmaxLower()
{
}

int SoftmaxLower::isMe(const ::onnx::Node& pNode) const
{
}

ComputeOperator*
SoftmaxLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
