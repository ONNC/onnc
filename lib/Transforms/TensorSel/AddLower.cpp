//===- AddLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/AddLower.h>

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
