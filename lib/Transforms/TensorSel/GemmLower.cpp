//===- GemmLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/GemmLower.h>
#include <onnc/IR/Compute/Gemm.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

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
  // check input/output number
  if (pNode.inputs().size() != 3)
    return nullptr;

  if (pNode.outputs().size() != 1)
    return nullptr;

  // check input/output name
  for (::onnx::Value* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  for (::onnx::Value* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // check default attributes
  

  // create operators
  onnc::Gemm* op = pGraph.addOperator<onnc::Gemm>();

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("alpha")))
    op->setAlpha(pNode.f(::onnx::Symbol("alpha")));
  if (pNode.hasAttribute(::onnx::Symbol("beta")))
    op->setBeta(pNode.f(::onnx::Symbol("beta")));
  if (pNode.hasAttribute(::onnx::Symbol("transA")))
    op->setTransA(pNode.i(::onnx::Symbol("transA")));
  if (pNode.hasAttribute(::onnx::Symbol("transB")))
    op->setTransB(pNode.i(::onnx::Symbol("transB")));

  // set input/output
  for (::onnx::Value* xv : pNode.inputs()) {
    onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addInput(*tensor);
  }

  for (::onnx::Value* xv : pNode.outputs()) {
    onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addOutput(*tensor);
  }
  return op;
}
