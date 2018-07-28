//===- AveragePool.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#define DEBUG_TYPE "tg_averagepool"
#include "AveragePool.h"
#include "../BM188xVisitor.h"

using namespace onnc;
using namespace onnc::BM188X;

char BM188X::AveragePool::ID = 0;

//===----------------------------------------------------------------------===//
// AveragePool
//===----------------------------------------------------------------------===//
BM188X::AveragePool::AveragePool(const IntsAttr& pKernelShape)
    : onnc::AveragePool(pKernelShape), m_EnableRelu(0), m_RShiftWidth(0),
      m_ThresholdXQuantized(0)
{
  setID(ID);
}

void BM188X::AveragePool::print(std::ostream& pOS) const
{
  // TODO
}

void BM188X::AveragePool::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::AveragePool::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool BM188X::AveragePool::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
