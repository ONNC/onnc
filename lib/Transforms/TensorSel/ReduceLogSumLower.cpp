//===- ReduceLogSumLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/ReduceLogSumLower.h>
#include <onnc/IR/Compute/ReduceLogSum.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReduceLogSumLower
//===----------------------------------------------------------------------===//
ReduceLogSumLower::ReduceLogSumLower()
{
}

ReduceLogSumLower::~ReduceLogSumLower()
{
}

int ReduceLogSumLower::isMe(const xNode& pNode) const
{
  if (pNode.kind() == xSymbol("ReduceLogSum"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
ReduceLogSumLower::activate(ComputeGraph& pGraph, xNode& pNode) const
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
  onnc::ReduceLogSum* op = pGraph.addOperator<onnc::ReduceLogSum>();

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("axes")))
    op->setAxes(pNode.is(xSymbol("axes")));
  if (pNode.hasAttribute(xSymbol("keepdims")))
    op->setKeepdims(pNode.i(xSymbol("keepdims")));

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
