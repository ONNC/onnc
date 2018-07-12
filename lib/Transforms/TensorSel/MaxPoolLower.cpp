//===- MaxPoolLower.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_MAXPOOLLOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_MAXPOOLLOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transfroms/TensorSel/Standards/MaxPoolLower.h>

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
