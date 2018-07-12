//===- LRNLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_LRNLOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_LRNLOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transfroms/TensorSel/Standards/LRNLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// LRNLower
//===----------------------------------------------------------------------===//
LRNLower::LRNLower()
{
}

LRNLower::~LRNLower()
{
}

int LRNLower::isMe(const ::onnx::Node& pNode) const
{
}

ComputeOperator*
LRNLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
