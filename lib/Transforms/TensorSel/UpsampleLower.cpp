//===- UpsampleLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/UpsampleLower.h>
#include <onnc/IR/Compute/Upsample.h>
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// UpsampleLower
//===----------------------------------------------------------------------===//
UpsampleLower::UpsampleLower()
{
}

UpsampleLower::~UpsampleLower()
{
}

int UpsampleLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("Upsample"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
UpsampleLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
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
  if (!pNode.hasAttribute(::onnx::Symbol("height_scale")))
    return nullptr;
  if (!pNode.hasAttribute(::onnx::Symbol("width_scale")))
    return nullptr;

  // create operators
  onnc::Upsample* op = pGraph.addOperator<onnc::Upsample>(
      pNode.fs(::onnx::Symbol("scales")));

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("mode")))
    op->setMode(pNode.s(::onnx::Symbol("mode")));

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
