//===- SoftmaxLower.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_SOFTMAXLOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_SOFTMAXLOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transfroms/TensorSel/Standards/SoftmaxLower.h>

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
