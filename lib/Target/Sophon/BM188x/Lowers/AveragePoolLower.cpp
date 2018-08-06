//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#include "AveragePoolLower.h"
#include "../Compute/AveragePool.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// AveragePoolLower
//===----------------------------------------------------------------------===//
BM188X::AveragePoolLower::AveragePoolLower()
{
}

BM188X::AveragePoolLower::~AveragePoolLower()
{
}

int BM188X::AveragePoolLower::isMe(const xNode &pNode) const
{
  if (pNode.kind() == xSymbol("AveragePool"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *
BM188X::AveragePoolLower::activate(ComputeGraph& pGraph,
                                   xNode &pNode) const
{
  // check input/output name
  if (1 != pNode.inputs().size())
    return nullptr;

  for (xValue* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  if (1 != pNode.outputs().size())
    return nullptr;

  for (xValue* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // check required attributes
  if (!pNode.hasAttribute(xSymbol("kernel_shape")))
    return nullptr;

  // create operators
  BM188X::AveragePool* op = pGraph.addOperator<onnc::BM188X::AveragePool>(
      pNode.is(xSymbol("kernel_shape")));

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("auto_pad")))
    op->setAutoPad(pNode.s(xSymbol("auto_pad")));

  if (pNode.hasAttribute(xSymbol("count_include_pad")))
    op->setCountIncludePad(pNode.i(xSymbol("count_include_pad")));

  if (pNode.hasAttribute(xSymbol("pads")))
    op->setPads(pNode.is(xSymbol("pads")));

  if (pNode.hasAttribute(xSymbol("strides")))
    op->setStrides(pNode.is(xSymbol("strides")));

  if (pNode.hasAttribute(xSymbol("enable_relu")))
    op->setEnableRelu(pNode.i(xSymbol("enable_relu")));

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
