//===- ConcatLower.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_CONCATLOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_CONCATLOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transfroms/TensorSel/Standards/ConcatLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ConcatLower
//===----------------------------------------------------------------------===//
ConcatLower::ConcatLower()
{
}

ConcatLower::~ConcatLower()
{
}

int ConcatLower::isMe(const ::onnx::Node& pNode) const
{
}

ComputeOperator*
ConcatLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
