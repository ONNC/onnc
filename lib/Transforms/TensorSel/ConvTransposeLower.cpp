//===- ConvTransposeLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/ConvTransposeLower.h>
#include <onnc/IR/Compute/ConvTranspose.h>
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ConvTransposeLower
//===----------------------------------------------------------------------===//
ConvTransposeLower::ConvTransposeLower()
{
}

ConvTransposeLower::~ConvTransposeLower()
{
}

int ConvTransposeLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("ConvTranspose"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
ConvTransposeLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() < 2 || 3 < pNode.inputs().size())
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
  onnc::ConvTranspose* op = pGraph.addOperator<onnc::ConvTranspose>();

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("auto_pad")))
    op->setAutoPad(pNode.s(::onnx::Symbol("auto_pad")));
  if (pNode.hasAttribute(::onnx::Symbol("dilations")))
    op->setDilations(pNode.is(::onnx::Symbol("dilations")));
  if (pNode.hasAttribute(::onnx::Symbol("group")))
    op->setGroup(pNode.i(::onnx::Symbol("group")));
  if (pNode.hasAttribute(::onnx::Symbol("kernel_shape")))
    op->setKernelShape(pNode.is(::onnx::Symbol("kernel_shape")));
  if (pNode.hasAttribute(::onnx::Symbol("output_padding")))
    op->setOutputPadding(pNode.is(::onnx::Symbol("output_padding")));
  if (pNode.hasAttribute(::onnx::Symbol("output_shape")))
    op->setOutputShape(pNode.is(::onnx::Symbol("output_shape")));
  if (pNode.hasAttribute(::onnx::Symbol("pads")))
    op->setPads(pNode.is(::onnx::Symbol("pads")));
  if (pNode.hasAttribute(::onnx::Symbol("strides")))
    op->setStrides(pNode.is(::onnx::Symbol("strides")));

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
