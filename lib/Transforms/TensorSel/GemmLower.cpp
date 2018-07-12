//===- GemmLower.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/GemmLower.h>

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
