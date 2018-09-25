//===- ScanLower.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/ScanLower.h>
#include <onnc/IR/Compute/Scan.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ScanLower
//===----------------------------------------------------------------------===//
ScanLower::ScanLower()
{
}

ScanLower::~ScanLower()
{
}

int ScanLower::isMe(const xNode& pNode) const
{
  if (pNode.kind() == xSymbol("Scan"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
ScanLower::activate(ComputeGraph& pGraph, xNode& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() < 2)
    return nullptr;

  if (pNode.outputs().size() < 1)
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
  if (!pNode.hasAttribute(xSymbol("body")))
    return nullptr;
  if (!pNode.hasAttribute(xSymbol("num_scan_inputs")))
    return nullptr;

  // create operators
  onnc::Scan* op = pGraph.addOperator<onnc::Scan>(
    pNode.g(xSymbol("body")),
    pNode.i(xSymbol("num_scan_inputs")));

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("directions")))
    op->setDirections(pNode.is(xSymbol("directions")));

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
