//===- ConstantFillLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/ConstantFillLower.h>
#include <onnc/IR/Compute/ConstantFill.h>
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ConstantFillLower
//===----------------------------------------------------------------------===//
ConstantFillLower::ConstantFillLower()
{
}

ConstantFillLower::~ConstantFillLower()
{
}

int ConstantFillLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("ConstantFill"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
ConstantFillLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() < 0 || 1 < pNode.inputs().size())
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
  onnc::ConstantFill* op = pGraph.addOperator<onnc::ConstantFill>();

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("dtype")))
    op->setDtype(pNode.i(::onnx::Symbol("dtype")));
  if (pNode.hasAttribute(::onnx::Symbol("extra_shape")))
    op->setExtraShape(pNode.is(::onnx::Symbol("extra_shape")));
  if (pNode.hasAttribute(::onnx::Symbol("input_as_shape")))
    op->setInputAsShape(pNode.i(::onnx::Symbol("input_as_shape")));
  if (pNode.hasAttribute(::onnx::Symbol("shape")))
    op->setShape(pNode.is(::onnx::Symbol("shape")));
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
