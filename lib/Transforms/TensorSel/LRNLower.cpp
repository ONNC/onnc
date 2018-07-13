//===- LRNLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/LRNLower.h>
#include <onnc/IR/Compute/LRN.h>

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
  if (pNode.kind() == ::onnx::Symbol("LRN"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
LRNLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
  // check input/output name
  if (1 != pNode.inputs().size())
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

  // check required attributes
  if (!pNode.hasAttribute(::onnx::Symbol("size")))
    return nullptr;

  // create operators
  onnc::LRN* op = pGraph.addOperator<onnc::LRN>(pNode.i(::onnx::Symbol("size")));

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("alpha")))
    op->setAlpha(pNode.f(::onnx::Symbol("alpha")));
  if (pNode.hasAttribute(::onnx::Symbol("beta")))
    op->setBeta(pNode.f(::onnx::Symbol("beta")));
  if (pNode.hasAttribute(::onnx::Symbol("bias")))
    op->setBias(pNode.f(::onnx::Symbol("bias")));

  // set input/output
  for (::onnx::Value* xv : pNode.inputs())
    op->addInput(*pGraph.getValue<onnc::Tensor>(xv->uniqueName()));

  for (::onnx::Value* xv : pNode.outputs())
    op->addOutput(*pGraph.getValue<onnc::Tensor>(xv->uniqueName()));

  return op;
}
