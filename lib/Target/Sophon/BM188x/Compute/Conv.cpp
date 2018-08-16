//===- Conv.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Conv.h"
#include "../BM188xVisitor.h"

#define DEBUG_TYPE "tg_conv"

using namespace onnc;
using namespace onnc::BM188X;

char BM188X::Conv::ID = 0;

//===----------------------------------------------------------------------===//
// Conv
//===----------------------------------------------------------------------===//
BM188X::Conv::Conv()
    : onnc::Conv(),  m_DoBias(0), /* m_DoRelu(0),*/ m_DoScale(0),
      m_DoScaleBias(0), m_RShiftWidth(0), m_ScaleRShiftWidth(0),
      m_ConvOutputThreshold(0.f)
{
  setID(ID);
}

BM188X::Conv::~Conv()
{
}

void BM188X::Conv::print(std::ostream& pOS) const
{
  // TODO
}

void BM188X::Conv::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::Conv::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool BM188X::Conv::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
