//===- BMScaleLower.cpp ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "BMScaleLower.h"
#include "../Compute/BMScale.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// BMScaleLower
//===----------------------------------------------------------------------===//
BM188X::BMScaleLower::BMScaleLower() {}

BM188X::BMScaleLower::~BMScaleLower() {}

int BM188X::BMScaleLower::isMe(const xNode &pNode) const
{
  if (pNode.kind() == xSymbol("TGScale"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *BM188X::BMScaleLower::activate(ComputeGraph &pGraph,
                                                      xNode &pNode) const
{
  // check input/output name
  if (3 != pNode.inputs().size())
    return nullptr;

  for (xValue *xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  if (1 != pNode.outputs().size())
    return nullptr;

  for (xValue *xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // create operators
  BM188X::BMScale *op = pGraph.addOperator<BM188X::BMScale>();

  // set input/output
  for (xValue *xv : pNode.inputs()) {
    onnc::Tensor *tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addInput(*tensor);
  }

  for (xValue *xv : pNode.outputs()) {
    onnc::Tensor *tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addOutput(*tensor);
  }
  return op;
}
