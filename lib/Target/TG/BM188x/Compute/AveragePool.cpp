//===- AveragePool.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#define DEBUG_TYPE "tg_averagepool"
#include "AveragePool.h"

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// AveragePool
//===----------------------------------------------------------------------===//
BM188X::AveragePool::AveragePool(const IntsAttr& pKernelShape)
    : onnc::AveragePool(pKernelShape), m_EnableRelu(0), m_RShiftWidth(0),
      m_ThresholdXQuantized(0)
{
  // do nothing
}

void BM188X::AveragePool::print(std::ostream& pOS) const
{
  // TODO
}
