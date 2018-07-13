//===- GemmLower.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/GemmLower.h>
#include <onnc/IR/Compute/Gemm.h>

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
  if (pNode.kind() == ::onnx::Symbol("Gemm"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
GemmLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
  // check input/output name
  if (3 == pNode.inputs().size())
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
  onnc::Gemm* op = pGraph.addOperator<onnc::Gemm>();

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("alpha")))
    op->setAlpha(pNode.f(::onnx::Symbol("alpha")));
  if (pNode.hasAttribute(::onnx::Symbol("beta")))
    op->setBeta(pNode.f(::onnx::Symbol("beta")));
  if (pNode.hasAttribute(::onnx::Symbol("broadcast")))
    op->setBroadcast(pNode.i(::onnx::Symbol("broadcast")));
  if (pNode.hasAttribute(::onnx::Symbol("transA")))
    op->setTransA(pNode.i(::onnx::Symbol("transA")));
  if (pNode.hasAttribute(::onnx::Symbol("transB")))
    op->setTransB(pNode.i(::onnx::Symbol("transB")));

  // set input/output
  for (::onnx::Value* xv : pNode.inputs())
    op->addInput(*pGraph.getValue<onnc::Tensor>(xv->uniqueName()));

  for (::onnx::Value* xv : pNode.outputs())
    op->addOutput(*pGraph.getValue<onnc::Tensor>(xv->uniqueName()));

  return op;
}
