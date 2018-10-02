//===- GRULower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/GRULower.h>
#include <onnc/IR/Compute/GRU.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// GRULower
//===----------------------------------------------------------------------===//
GRULower::GRULower()
{
}

GRULower::~GRULower()
{
}

int GRULower::isMe(const xNode& pNode) const
{
  if (pNode.kind() == xSymbol("GRU"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
GRULower::activate(ComputeGraph& pGraph, xNode& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() < 3 || 6 < pNode.inputs().size())
    return nullptr;

  if (pNode.outputs().size() < 0 || 2 < pNode.outputs().size())
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
  onnc::GRU* op = pGraph.addOperator<onnc::GRU>();

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("activation_alpha")))
    op->setActivationAlpha(pNode.fs(xSymbol("activation_alpha")));
  if (pNode.hasAttribute(xSymbol("activation_beta")))
    op->setActivationBeta(pNode.fs(xSymbol("activation_beta")));
  if (pNode.hasAttribute(xSymbol("activations")))
    op->setActivations(pNode.ss(xSymbol("activations")));
  if (pNode.hasAttribute(xSymbol("clip")))
    op->setClip(pNode.f(xSymbol("clip")));
  if (pNode.hasAttribute(xSymbol("direction")))
    op->setDirection(pNode.s(xSymbol("direction")));
  if (pNode.hasAttribute(xSymbol("hidden_size")))
    op->setHiddenSize(pNode.i(xSymbol("hidden_size")));
  if (pNode.hasAttribute(xSymbol("linear_before_reset")))
    op->setLinearBeforeReset(pNode.i(xSymbol("linear_before_reset")));

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
