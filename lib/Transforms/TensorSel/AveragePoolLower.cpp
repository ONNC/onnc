//===- AveragePoolLower.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/AveragePoolLower.h>

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
}

ComputeOperator*
AveragePoolLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
