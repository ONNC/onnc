//===- MaxPoolLower.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "MaxPoolLower.h"
#include "../Compute/MaxPool.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// MaxPoolLower
//===----------------------------------------------------------------------===//
BM188X::MaxPoolLower::MaxPoolLower()
{
}

BM188X::MaxPoolLower::~MaxPoolLower()
{
}

int BM188X::MaxPoolLower::isMe(const xNode &pNode) const
{
  if (pNode.kind() == xSymbol("MaxPool"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *
BM188X::MaxPoolLower::activate(ComputeGraph& pGraph, xNode &pNode) const
{
  // check input/output name
  if (1 != pNode.inputs().size())
    return nullptr;

  for (xValue* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  for (xValue* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // check required attributes
  if (!pNode.hasAttribute(xSymbol("kernel_shape")))
    return nullptr;

  // create operators
  BM188X::MaxPool* op = pGraph.addOperator<BM188X::MaxPool>(
      pNode.is(xSymbol("kernel_shape")));

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("pads")))
    op->setPads(pNode.is(xSymbol("pads")));

  if (pNode.hasAttribute(xSymbol("strides")))
    op->setStrides(pNode.is(xSymbol("strides")));

  // set input/output
  for (xValue* xv : pNode.inputs()) {
    onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addInput(*tensor);
  }

  for (xValue* xv : pNode.outputs()) {
    onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addOutput(*tensor);
  }
  return op;
}
