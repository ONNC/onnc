//===- LRNLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "LRNLower.h"
#include "../Compute/LRN.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// LRNLower
//===----------------------------------------------------------------------===//
BM188X::LRNLower::LRNLower()
{
}

BM188X::LRNLower::~LRNLower()
{
}

int BM188X::LRNLower::isMe(const ::onnx::Node &pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("LRN"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *BM188X::LRNLower::activate(ComputeGraph& pGraph,
                                                   ::onnx::Node &pNode) const
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
  BM188X::LRN *op =
    pGraph.addOperator<onnc::BM188X::LRN>(pNode.i(::onnx::Symbol("size")));

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("alpha")))
    op->setAlpha(pNode.f(::onnx::Symbol("alpha")));
  if (pNode.hasAttribute(::onnx::Symbol("beta")))
    op->setBeta(pNode.f(::onnx::Symbol("beta")));
  if (pNode.hasAttribute(::onnx::Symbol("bias")))
    op->setBias(pNode.f(::onnx::Symbol("bias")));

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