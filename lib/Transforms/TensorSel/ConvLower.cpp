//===- ConvLower.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_CONVLOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_CONVLOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transfroms/TensorSel/Standards/ConvLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ConvLower
//===----------------------------------------------------------------------===//
ConvLower::ConvLower()
{
}

ConvLower::~ConvLower()
{
}

int ConvLower::isMe(const ::onnx::Node& pNode) const
{
}

ComputeOperator*
ConvLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
