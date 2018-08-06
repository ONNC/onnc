//===- BatchNormalizationLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/BatchNormalizationLower.h>
#include <onnc/IR/Compute/BatchNormalization.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// BatchNormalizationLower
//===----------------------------------------------------------------------===//
BatchNormalizationLower::BatchNormalizationLower()
{
}

BatchNormalizationLower::~BatchNormalizationLower()
{
}

int BatchNormalizationLower::isMe(const xNode& pNode) const
{
  if (pNode.kind() == xSymbol("BatchNormalization"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
BatchNormalizationLower::activate(ComputeGraph& pGraph, xNode& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() != 5)
    return nullptr;

  if (pNode.outputs().size() < 1 || 5 < pNode.outputs().size())
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
  onnc::BatchNormalization* op = pGraph.addOperator<onnc::BatchNormalization>();

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("epsilon")))
    op->setEpsilon(pNode.f(xSymbol("epsilon")));
  if (pNode.hasAttribute(xSymbol("momentum")))
    op->setMomentum(pNode.f(xSymbol("momentum")));
  if (pNode.hasAttribute(xSymbol("spatial")))
    op->setSpatial(pNode.i(xSymbol("spatial")));

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
