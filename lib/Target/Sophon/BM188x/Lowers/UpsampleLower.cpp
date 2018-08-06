//===- UpsampleLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "UpsampleLower.h"
#include "../Compute/Upsample.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// UpsampleLower
//===----------------------------------------------------------------------===//
BM188X::UpsampleLower::UpsampleLower()
{
}

BM188X::UpsampleLower::~UpsampleLower()
{
}

int BM188X::UpsampleLower::isMe(const xNode& pNode) const
{
  if (pNode.kind() == xSymbol("Upsample"))
    return kStdLower;
  return kNotMe;
}

static bool isValidSize(const IntsAttr& pSize)
{
  return (pSize.vector()[0] == 1 && pSize.vector()[1] == 1 &&
          (pSize.vector()[2] == pSize.vector()[3]));
}

ComputeOperator*
BM188X::UpsampleLower::activate(ComputeGraph& pGraph, xNode& pNode) const
{
  // check input/output name
  if (1 != pNode.inputs().size() ||
      1 != pNode.outputs().size())
    return nullptr;

  for (xValue* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  for (xValue* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // check required attributes
  if (!pNode.hasAttribute(xSymbol("height_scale")) ||
      !pNode.hasAttribute(xSymbol("width_scale")))
    return nullptr;

  // check customized attribute.
  IntsAttr sizes = pNode.is(xSymbol("size"));
  if (!isValidSize(sizes)) {
    errs() << "BM188X::UpsampleLower: Invalid size.\n";
    return nullptr;
  }

  // create operators
  BM188X::Upsample* op = pGraph.addOperator<BM188X::Upsample>(
      sizes.vector()[3],
      pNode.f(xSymbol("height_scale")),
      pNode.f(xSymbol("width_scale")));

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("mode")))
    op->setMode(pNode.s(xSymbol("mode")));

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
