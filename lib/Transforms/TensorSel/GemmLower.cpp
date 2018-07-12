//===- GemmLower.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_GEMMLOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_GEMMLOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transfroms/TensorSel/Standards/GemmLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// GemmLower
//===----------------------------------------------------------------------===//
GemmLower::GemmLower()
{
}

GemmLower::~GemmLower()
{
}

int GemmLower::isMe(const ::onnx::Node& pNode) const
{
}

ComputeOperator*
GemmLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
