//===- TransposeLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "TransposeLower.h"
#include "../Compute/Transpose.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// TransposeLower
//===----------------------------------------------------------------------===//
BM188X::TransposeLower::TransposeLower()
{
}

BM188X::TransposeLower::~TransposeLower()
{
}

int BM188X::TransposeLower::isMe(const xNode &pNode) const
{
  if (pNode.kind() == xSymbol("Transpose"))
    return kTargetNormal;
  return kNotMe;
}

static bool isValidOrder(const std::vector<int64_t> &pOrders)
{
  // Pattern:
  //   0 2 3 1
  //   2 3 0 1
  //   1 0 2 3
  //   0 2 1 3 4
  return (((pOrders[0] == 0) && (pOrders[1] == 2) && (pOrders[2] == 3) &&
          (pOrders[3] == 1)) ||
         ((pOrders[0] == 2) && (pOrders[1] == 3) && (pOrders[2] == 0) &&
          (pOrders[3] == 1)) ||
         ((pOrders[0] == 1) && (pOrders[1] == 0) && (pOrders[2] == 2) &&
          (pOrders[3] == 3)) ||
         ((pOrders[0] == 0) && (pOrders[1] == 2) && (pOrders[2] == 1) &&
          (pOrders[3] == 3) && (pOrders[4] == 4)));
}

onnc::ComputeOperator *BM188X::TransposeLower::activate(ComputeGraph& pGraph,
                                                      xNode &pNode) const
{
  // check input/output name
  if (1 != pNode.inputs().size() ||
      1 != pNode.outputs().size())
    return nullptr;

  xValue* ox = pNode.inputs()[0];
  xValue* oy = pNode.outputs()[0];

  if (!ox->has_unique_name() ||
      !oy->has_unique_name())
    return nullptr;

  auto &orders = pNode.is(xSymbol("perm"));
  if ((orders.size() != 4) && (orders.size() != 5)) {
    errs() << "BM188X::TransposeLower: Invalid perm\n";
    return nullptr;
  }

  if (!isValidOrder(orders)) {
    errs() << "BM188X::TransposeLower: Invalid order\n";
    return nullptr;
  }

  // create operators
  BM188X::Transpose *op = pGraph.addOperator<onnc::BM188X::Transpose>();

  op->init(pNode);

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
