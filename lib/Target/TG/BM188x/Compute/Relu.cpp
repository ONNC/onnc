//===- Relu.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Relu.h"
#include "../BM188xVisitor.h"
#include <onnc/Support/Casting.h>

#define DEBUG_TYPE "tg_relu"

using namespace onnc;

//===----------------------------------------------------------------------===//
// BM188X::Relu
//===----------------------------------------------------------------------===//
BM188X::Relu::Relu()
    : m_NegativeSlope(0.0)
{
}

void BM188X::Relu::print(std::ostream& pOS) const
{
  // TODO
}

void BM188X::Relu::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::Relu::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

