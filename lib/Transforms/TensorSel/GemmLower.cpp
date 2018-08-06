//===- GemmLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/GemmLower.h>
#include <onnc/IR/Compute/Gemm.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// GemmLower
//===----------------------------------------------------------------------===//
GemmLower::GemmLower()
{
}

GemmLower::~GemmLower()
{
}

int GemmLower::isMe(const xNode& pNode) const
{
  if (pNode.kind() == xSymbol("Gemm"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
GemmLower::activate(ComputeGraph& pGraph, xNode& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() != 3)
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
  onnc::Gemm* op = pGraph.addOperator<onnc::Gemm>();

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("alpha")))
    op->setAlpha(pNode.f(xSymbol("alpha")));
  if (pNode.hasAttribute(xSymbol("beta")))
    op->setBeta(pNode.f(xSymbol("beta")));
  if (pNode.hasAttribute(xSymbol("transA")))
    op->setTransA(pNode.i(xSymbol("transA")));
  if (pNode.hasAttribute(xSymbol("transB")))
    op->setTransB(pNode.i(xSymbol("transB")));

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
