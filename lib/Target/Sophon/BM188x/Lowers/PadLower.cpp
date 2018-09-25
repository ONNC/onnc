//===- PadLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "PadLower.h"

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// PadLower
//===----------------------------------------------------------------------===//
BM188X::PadLower::PadLower()
{
}

BM188X::PadLower::~PadLower()
{
}

int BM188X::PadLower::isMe(const xNode &pNode) const
{
  if (pNode.kind() == xSymbol("Pad")) {
    return kTargetNormal;
  }

  return kNotMe;
}

onnc::ComputeOperator *
BM188X::PadLower::activate(ComputeGraph& pGraph, xNode &pNode) const
{
  errs() << "Doesn't support pad op yet.\n";
  assert(0);

  return nullptr;
}
