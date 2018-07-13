//===- AddLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/AddLower.h>
#include <onnc/IR/Compute/Add.h>

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
  if (pNode.kind() == ::onnx::Symbol("Add"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
AddLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
  // check input/output name
  if (2 != pNode.inputs().size())
    return nullptr;

  for (::onnx::Value* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  if (1 != pNode.outputs().size())
    return nullptr;

  for (::onnx::Value* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // create operators
  onnc::Add* op = pGraph.addOperator<onnc::Add>();

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("axis")))
    op->setAxis(pNode.i(::onnx::Symbol("axis")));
  if (pNode.hasAttribute(::onnx::Symbol("broadcast")))
    op->setBroadcast(pNode.i(::onnx::Symbol("broadcast")));

  // set input/output
  onnc::Tensor* a =
     pGraph.getValue<onnc::Tensor>(pNode.inputs()[onnc::Add::kA]->uniqueName());
  onnc::Tensor* b =
     pGraph.getValue<onnc::Tensor>(pNode.inputs()[onnc::Add::kB]->uniqueName());
  onnc::Tensor* c =
    pGraph.getValue<onnc::Tensor>(pNode.outputs()[onnc::Add::kC]->uniqueName());

  op->setA(*a);
  op->setB(*b);
  op->setC(*c);
  return op;
}
