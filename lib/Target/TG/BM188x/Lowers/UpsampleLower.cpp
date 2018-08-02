//===- UpsampleLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "UpsampleLower.h"
#include "../Compute/Upsample.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// UpsampleLower
//===----------------------------------------------------------------------===//
BM188X::UpsampleLower::UpsampleLower()
{
}

BM188X::UpsampleLower::~UpsampleLower()
{
}

int BM188X::UpsampleLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("Upsample"))
    return kStdLower;
  return kNotMe;
}

static bool isValidSize(const IntsAttr& pSize)
{
  return (pSize.vector()[0] == 1 && pSize.vector()[1] == 1 &&
          (pSize.vector()[2] == pSize.vector()[3]));
}

ComputeOperator*
BM188X::UpsampleLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
  // check input/output name
  if (1 != pNode.inputs().size() ||
      1 != pNode.outputs().size())
    return nullptr;

  for (::onnx::Value* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  for (::onnx::Value* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // check required attributes
  if (!pNode.hasAttribute(::onnx::Symbol("height_scale")) ||
      !pNode.hasAttribute(::onnx::Symbol("width_scale")))
    return nullptr;

  // check customized attribute.
  IntsAttr sizes = pNode.is(::onnx::Symbol("size"));
  if (!isValidSize(sizes)) {
    errs() << "BM188X::UpsampleLower: Invalid size.\n";
    return nullptr;
  }

  // create operators
  BM188X::Upsample* op = pGraph.addOperator<BM188X::Upsample>(
      sizes.vector()[3],
      pNode.f(::onnx::Symbol("height_scale")),
      pNode.f(::onnx::Symbol("width_scale")));

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
