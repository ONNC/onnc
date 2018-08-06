//===- ScaleLower.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ScaleLower.h"
#include "../Compute/Scale.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ScaleLower
//===----------------------------------------------------------------------===//
BM188X::ScaleLower::ScaleLower()
{
}

BM188X::ScaleLower::~ScaleLower()
{
}

int BM188X::ScaleLower::isMe(const xNode &pNode) const
{
  if (pNode.kind() == xSymbol("Scale"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *
BM188X::ScaleLower::activate(ComputeGraph& pGraph, xNode &pNode) const
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
  if (!pNode.hasAttribute(xSymbol("scale")))
    return nullptr;
  
  // create operators
  BM188X::Scale* op = pGraph.addOperator<BM188X::Scale>();

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

