//===- ConvTransposeLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/ConvTransposeLower.h>
#include <onnc/IR/Compute/ConvTranspose.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ConvTransposeLower
//===----------------------------------------------------------------------===//
ConvTransposeLower::ConvTransposeLower()
{
}

ConvTransposeLower::~ConvTransposeLower()
{
}

int ConvTransposeLower::isMe(const xNode& pNode) const
{
  if (pNode.kind() == xSymbol("ConvTranspose"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
ConvTransposeLower::activate(ComputeGraph& pGraph, xNode& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() < 2 || 3 < pNode.inputs().size())
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
  onnc::ConvTranspose* op = pGraph.addOperator<onnc::ConvTranspose>();

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("auto_pad")))
    op->setAutoPad(pNode.s(xSymbol("auto_pad")));
  if (pNode.hasAttribute(xSymbol("dilations")))
    op->setDilations(pNode.is(xSymbol("dilations")));
  if (pNode.hasAttribute(xSymbol("group")))
    op->setGroup(pNode.i(xSymbol("group")));
  if (pNode.hasAttribute(xSymbol("kernel_shape")))
    op->setKernelShape(pNode.is(xSymbol("kernel_shape")));
  if (pNode.hasAttribute(xSymbol("output_padding")))
    op->setOutputPadding(pNode.is(xSymbol("output_padding")));
  if (pNode.hasAttribute(xSymbol("output_shape")))
    op->setOutputShape(pNode.is(xSymbol("output_shape")));
  if (pNode.hasAttribute(xSymbol("pads")))
    op->setPads(pNode.is(xSymbol("pads")));
  if (pNode.hasAttribute(xSymbol("strides")))
    op->setStrides(pNode.is(xSymbol("strides")));

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
