//===- ConcatLower.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/ConcatLower.h>

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
