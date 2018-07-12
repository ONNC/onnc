//===- AddLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_ADDLOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_ADDLOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transfroms/TensorSel/Standards/AddLower.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// AddLower
//===----------------------------------------------------------------------===//
AddLower::AddLower()
{
}

AddLower::~AddLower()
{
}

int AddLower::isMe(const ::onnx::Node& pNode) const
{
}

ComputeOperator*
AddLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
}
