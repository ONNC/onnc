//===- ConstantFillLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/ConstantFillLower.h>
#include <onnc/IR/Compute/ConstantFill.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ConstantFillLower
//===----------------------------------------------------------------------===//
ConstantFillLower::ConstantFillLower()
{
}

ConstantFillLower::~ConstantFillLower()
{
}

int ConstantFillLower::isMe(const xNode& pNode) const
{
  if (pNode.kind() == xSymbol("ConstantFill"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
ConstantFillLower::activate(ComputeGraph& pGraph, xNode& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() < 0 || 1 < pNode.inputs().size())
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
  onnc::ConstantFill* op = pGraph.addOperator<onnc::ConstantFill>();

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("dtype")))
    op->setDtype(pNode.i(xSymbol("dtype")));
  if (pNode.hasAttribute(xSymbol("extra_shape")))
    op->setExtraShape(pNode.is(xSymbol("extra_shape")));
  if (pNode.hasAttribute(xSymbol("input_as_shape")))
    op->setInputAsShape(pNode.i(xSymbol("input_as_shape")));
  if (pNode.hasAttribute(xSymbol("shape")))
    op->setShape(pNode.is(xSymbol("shape")));
  if (pNode.hasAttribute(xSymbol("value")))
    op->setValue(pNode.f(xSymbol("value")));

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
