//===- RandomUniformLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/RandomUniformLower.h>
#include <onnc/IR/Compute/RandomUniform.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// RandomUniformLower
//===----------------------------------------------------------------------===//
RandomUniformLower::RandomUniformLower()
{
}

RandomUniformLower::~RandomUniformLower()
{
}

int RandomUniformLower::isMe(const xNode& pNode) const
{
  if (pNode.kind() == xSymbol("RandomUniform"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
RandomUniformLower::activate(ComputeGraph& pGraph, xNode& pNode) const
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
  onnc::RandomUniform* op = pGraph.addOperator<onnc::RandomUniform>(
    pNode.is(xSymbol("shape")));

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("dtype")))
    op->setDtype(pNode.i(xSymbol("dtype")));
  if (pNode.hasAttribute(xSymbol("high")))
    op->setHigh(pNode.f(xSymbol("high")));
  if (pNode.hasAttribute(xSymbol("low")))
    op->setLow(pNode.f(xSymbol("low")));
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
