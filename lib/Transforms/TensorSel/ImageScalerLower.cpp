//===- ImageScalerLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/ImageScalerLower.h>
#include <onnc/IR/Compute/ImageScaler.h>
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ImageScalerLower
//===----------------------------------------------------------------------===//
ImageScalerLower::ImageScalerLower()
{
}

ImageScalerLower::~ImageScalerLower()
{
}

int ImageScalerLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("ImageScaler"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
ImageScalerLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
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
  

  // create operators
  onnc::ImageScaler* op = pGraph.addOperator<onnc::ImageScaler>();

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("bias")))
    op->setBias(pNode.fs(::onnx::Symbol("bias")));
  if (pNode.hasAttribute(::onnx::Symbol("scale")))
    op->setScale(pNode.f(::onnx::Symbol("scale")));

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
