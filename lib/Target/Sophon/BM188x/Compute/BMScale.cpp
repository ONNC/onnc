//===- BMScale.cpp --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "BMScale.h"
#include "../BM188xVisitor.h"

#define DEBUG_TYPE "sophon_tgscale"

using namespace onnc;
using namespace onnc::BM188X;

char BM188X::BMScale::ID = 0;

//===----------------------------------------------------------------------===//
// BM188X::BMScale
//===----------------------------------------------------------------------===//
BM188X::BMScale::BMScale() : ComputeOperator("BMScale", ID), m_RShiftWidth(0) {}

void BM188X::BMScale::print(std::ostream &pOS) const
{
  // TODO
}

void BM188X::BMScale::accept(ComputeVisitor &pV)
{
  BM188xVisitor *visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::BMScale::accept(ComputeVisitor &pV) const
{
  BM188xVisitor *visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool BM188X::BMScale::classof(const ComputeOperator *pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
