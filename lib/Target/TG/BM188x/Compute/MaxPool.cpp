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

//===----------------------------------------------------------------------===//
// MaxPool
//===----------------------------------------------------------------------===//
BM188X::MaxPool::MaxPool()
    : ComputeOperator("MaxPool"),
      m_KernelShape(),
      m_Pads(),
      m_Strides(),
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
