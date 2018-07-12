//===- AveragePoolLower.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_AVERAGEPOOLLOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_AVERAGEPOOLLOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transfroms/TensorSel/Standards/AveragePoolLower.h>

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
