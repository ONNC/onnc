//===- PadLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/PadLower.h>
#include <onnc/IR/Compute/Pad.h>
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// PadLower
//===----------------------------------------------------------------------===//
PadLower::PadLower()
{
}

PadLower::~PadLower()
{
}

int PadLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("Pad"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
PadLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() != 1)
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
  if (!pNode.hasAttribute(::onnx::Symbol("pads")))
    return nullptr;

  // create operators
  onnc::Pad* op = pGraph.addOperator<onnc::Pad>(
    pNode.is(::onnx::Symbol("pads")));

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("mode")))
    op->setMode(pNode.s(::onnx::Symbol("mode")));
  if (pNode.hasAttribute(::onnx::Symbol("value")))
    op->setValue(pNode.f(::onnx::Symbol("value")));

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
