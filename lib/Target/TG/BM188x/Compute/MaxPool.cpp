//===- MaxPool.cpp --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "MaxPool.h"
#include "../BM188xVisitor.h"

#define DEBUG_TYPE "tg_maxpool"

using namespace onnc;
using namespace onnc::BM188X;

char BM188X::MaxPool::ID = 0;

//===----------------------------------------------------------------------===//
// MaxPool
//===----------------------------------------------------------------------===//
BM188X::MaxPool::MaxPool(const IntsAttr& pKS)
    : ComputeOperator("MaxPool", ID),
      m_KernelShape(pKS),
      m_Pads(IntsAttr(4,0)),
      m_Strides(IntsAttr(2, 1)),
      m_RShiftWidth(0),
      m_ThresholdXQuantized(0)
{
}

void BM188X::MaxPool::print(std::ostream& pOS) const
{
  // TODO
}

void BM188X::MaxPool::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::MaxPool::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool BM188X::MaxPool::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
