//===- BatchNormalizationLower.cpp ----------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_BATCHNORMALIZATIONLOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_BATCHNORMALIZATIONLOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transfroms/TensorSel/Standards/BatchNormalizationLower.h>

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
