//===- MaxPoolLower.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/MaxPoolLower.h>

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
}

ComputeOperator*
MaxPoolLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
