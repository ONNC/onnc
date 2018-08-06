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
  if (pNode.kind() == xSymbol("Conv"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *BM188X::ConvLower::activate(ComputeGraph& pGraph,
                                                   xNode &pNode) const
{
  // check input/output name
  if (1 > pNode.inputs().size() || pNode.inputs().size() > 4)
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

  if (pNode.hasAttribute(xSymbol("group")))
    op->setGroup(pNode.i(xSymbol("group")));
  if (pNode.hasAttribute(xSymbol("kernel_shape")))
    op->setKernelShape(pNode.is(xSymbol("kernel_shape")));
  if (pNode.hasAttribute(xSymbol("pads")))
    op->setPads(pNode.is(xSymbol("pads")));
  if (pNode.hasAttribute(xSymbol("strides")))
    op->setStrides(pNode.is(xSymbol("strides")));

  // set input/output
  for (xValue* xv : pNode.inputs())
    op->addInput(*pGraph.getValue<onnc::Tensor>(xv->uniqueName()));

  for (xValue* xv : pNode.outputs())
    op->addOutput(*pGraph.getValue<onnc::Tensor>(xv->uniqueName()));

  if (pm::match(&pNode, pm::mTrueAttr("do_scale")))
    op->setDoScale(1);

  if (pm::match(&pNode, pm::mTrueAttr("do_scale_bias")))
    op->setDoScaleBias(1);

  int idx = 3;
  if (pNode.inputs().size() - op->getDoScale() - op->getDoScaleBias() == 3) {
    op->setDoBias(1);
    op->setBiasIdx(idx++);
  }

  if (op->isDoScale()) {
    op->setScaleIdx(idx++);
    op->setConvOutputThreshold(pNode.f(xSymbol("conv_output_threshold")));
  }

  if (op->isDoScaleBias())
    op->setScaleBiasIdx(idx++);

  return op;
}
