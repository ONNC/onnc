//===- ReshapeLower.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_RESHAPELOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_RESHAPELOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transfroms/TensorSel/Standards/ReshapeLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReshapeLower
//===----------------------------------------------------------------------===//
ReshapeLower::ReshapeLower()
{
}

ReshapeLower::~ReshapeLower()
{
}

int ReshapeLower::isMe(const ::onnx::Node& pNode) const
{
}

ComputeOperator*
ReshapeLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
