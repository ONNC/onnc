//===- RandomNormalLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/RandomNormalLower.h>
#include <onnc/IR/Compute/RandomNormal.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// RandomNormalLower
//===----------------------------------------------------------------------===//
RandomNormalLower::RandomNormalLower()
{
}

RandomNormalLower::~RandomNormalLower()
{
}

int RandomNormalLower::isMe(const xNode& pNode) const
{
  if (pNode.kind() == xSymbol("RandomNormal"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
RandomNormalLower::activate(ComputeGraph& pGraph, xNode& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() != 0)
    return nullptr;

  if (pNode.outputs().size() != 1)
    return nullptr;

  // check input/output name
  for (xValue* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  for (xValue* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // check default attributes
  if (!pNode.hasAttribute(xSymbol("shape")))
    return nullptr;

  // create operators
  onnc::RandomNormal* op = pGraph.addOperator<onnc::RandomNormal>(
    pNode.is(xSymbol("shape")));

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("dtype")))
    op->setDtype(pNode.i(xSymbol("dtype")));
  if (pNode.hasAttribute(xSymbol("mean")))
    op->setMean(pNode.f(xSymbol("mean")));
  if (pNode.hasAttribute(xSymbol("scale")))
    op->setScale(pNode.f(xSymbol("scale")));
  if (pNode.hasAttribute(xSymbol("seed")))
    op->setSeed(pNode.f(xSymbol("seed")));

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
