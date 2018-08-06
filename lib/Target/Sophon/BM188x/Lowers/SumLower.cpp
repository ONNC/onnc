//===- SumLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "SumLower.h"
#include "../Compute/Sum.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// SumLower
//===----------------------------------------------------------------------===//
BM188X::SumLower::SumLower()
{
}

BM188X::SumLower::~SumLower()
{
}

int BM188X::SumLower::isMe(const xNode &pNode) const
{
  if (pNode.kind() == xSymbol("Sum"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *
BM188X::SumLower::activate(ComputeGraph& pGraph, xNode &pNode) const
{
  // check input/output name
  if (0 == pNode.inputs().size())
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

  // create operators
  BM188X::Sum* op = pGraph.addOperator<BM188X::Sum>();

  // init xquant with zero values.
  op->setThresholdXQuantized(std::vector<int>(pNode.inputs().size()));

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
