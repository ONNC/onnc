//===- SumLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_SUMLOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_SUMLOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transfroms/TensorSel/Standards/SumLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// SumLower
//===----------------------------------------------------------------------===//
SumLower::SumLower()
{
}

SumLower::~SumLower()
{
}

int SumLower::isMe(const ::onnx::Node& pNode) const
{
}

ComputeOperator*
SumLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
