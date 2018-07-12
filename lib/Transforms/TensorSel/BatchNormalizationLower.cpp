//===- BatchNormalizationLower.cpp ----------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/BatchNormalizationLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// BatchNormalizationLower
//===----------------------------------------------------------------------===//
BatchNormalizationLower::BatchNormalizationLower()
{
}

BatchNormalizationLower::~BatchNormalizationLower()
{
}

int BatchNormalizationLower::isMe(const ::onnx::Node& pNode) const
{
}

ComputeOperator*
BatchNormalizationLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
