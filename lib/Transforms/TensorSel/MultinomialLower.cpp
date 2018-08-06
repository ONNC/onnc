//===- MultinomialLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/MultinomialLower.h>
#include <onnc/IR/Compute/Multinomial.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// MultinomialLower
//===----------------------------------------------------------------------===//
MultinomialLower::MultinomialLower()
{
}

MultinomialLower::~MultinomialLower()
{
}

int MultinomialLower::isMe(const xNode& pNode) const
{
  if (pNode.kind() == xSymbol("Multinomial"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
MultinomialLower::activate(ComputeGraph& pGraph, xNode& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() != 1)
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
  

  // create operators
  onnc::Multinomial* op = pGraph.addOperator<onnc::Multinomial>();

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("dtype")))
    op->setDtype(pNode.i(xSymbol("dtype")));
  if (pNode.hasAttribute(xSymbol("sample_size")))
    op->setSampleSize(pNode.i(xSymbol("sample_size")));
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
