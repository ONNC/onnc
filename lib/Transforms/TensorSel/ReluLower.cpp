//===- ReluLower.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_RELULOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_RELULOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transfroms/TensorSel/Standards/ReluLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReluLower
//===----------------------------------------------------------------------===//
ReluLower::ReluLower()
{
}

ReluLower::~ReluLower()
{
}

int ReluLower::isMe(const ::onnx::Node& pNode) const
{
}

ComputeOperator*
ReluLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
