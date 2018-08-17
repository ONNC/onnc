//===- ConvLower.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ConvLower.h"
#include "PatternMatch.h"
#include "../Compute/Conv.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

namespace pm = onnc::PatternMatch;

//===----------------------------------------------------------------------===//
// ConvLower
//===----------------------------------------------------------------------===//
BM188X::ConvLower::ConvLower()
{
}

BM188X::ConvLower::~ConvLower()
{
}

int BM188X::ConvLower::isMe(const xNode &pNode) const
{
  if (pNode.kind() == xSymbol("Conv") ||
      pNode.kind() == xSymbol("TGConv"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *BM188X::ConvLower::activate(ComputeGraph& pGraph,
                                                   xNode &pNode) const
{
  // check input/output name
  if (1 > pNode.inputs().size())
    return nullptr;

  for (xValue* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  if (1 != pNode.outputs().size())
    return nullptr;

  for (xValue* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // create operators
  BM188X::Conv* op = pGraph.addOperator<onnc::BM188X::Conv>();

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("auto_pad")))
    op->setAutoPad(pNode.s(xSymbol("auto_pad")));

  if (pNode.hasAttribute(xSymbol("dilations")))
    op->setDilations(pNode.is(xSymbol("dilations")));
  else
    op->setDilations(IntsAttr(2, 1)); // {1, 1}

  if (pNode.hasAttribute(xSymbol("group")))
    op->setGroup(pNode.i(xSymbol("group")));
  else
    op->setGroup(IntAttr(1)); // {1}

  if (pNode.hasAttribute(xSymbol("kernel_shape")))
    op->setKernelShape(pNode.is(xSymbol("kernel_shape")));
  if (pNode.hasAttribute(xSymbol("pads")))
    op->setPads(pNode.is(xSymbol("pads")));
  else
    op->setPads(IntsAttr(2, 0)); // {0, 0}

  if (pNode.hasAttribute(xSymbol("strides")))
    op->setStrides(pNode.is(xSymbol("strides")));
  else
    op->setStrides(IntsAttr(2, 1)); // {1, 1}

  op->setDoRelu(pm::match(&pNode, pm::mTrueAttr("do_relu")));

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

  unsigned inputCnt = 2; // input, weight.
  bool doScale = pm::match(&pNode, pm::mTrueAttr("do_scale"));
  bool doScaleBias = pm::match(&pNode, pm::mTrueAttr("do_scale_bias"));

  if (doScale) ++inputCnt;
  if (doScaleBias) ++inputCnt;

  bool doBias = false;
  if (pNode.inputs().size() != inputCnt) {
    assert((pNode.inputs().size() - inputCnt == 1) &&
           "[BM188X::ConvLower] unexpected input cnt.");
    doBias = true;
  }

  int inputIdx = 2;
  if (doBias) {
    op->setBiasIdx(inputIdx++);
  }
  if (doScale) {
    op->setScaleIdx(inputIdx++);
  }
  if (doScaleBias) {
    op->setScaleBiasIdx(inputIdx++);
  }

  if (doScale)
    op->setConvOutputThreshold(pNode.f(xSymbol("conv_output_threshold")));

  return op;
}
