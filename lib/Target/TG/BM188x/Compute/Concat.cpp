//===- Concat.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#define DEBUG_TYPE "tg_concat"
#include "Concat.h"

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// Concat
//===----------------------------------------------------------------------===//
BM188X::Concat::Concat(const IntAttr& pAxis)
    : onnc::Concat(pAxis), m_NeedQuantizeNum(0), m_RShiftWidth(),
      m_ThresholdXQuantized()
{
}

BM188X::Concat::~Concat()
{
}

void BM188X::Concat::setRShiftWidth(unsigned int pIdx, int pValue)
{
  if (m_RShiftWidth.size() > pIdx)
    m_RShiftWidth[pIdx] = pValue;
}

const int* BM188X::Concat::getRShiftWidth() const
{
  return m_RShiftWidth.data();
}

void BM188X::Concat::setThresholdXQuantized(unsigned int pIdx, int pValue)
{
  if (m_ThresholdXQuantized.size() > pIdx)
    m_ThresholdXQuantized[pIdx] = pValue;
}

const int* BM188X::Concat::getThresholdXQuantized() const
{
  return m_ThresholdXQuantized.data();
}

void BM188X::Concat::print(std::ostream& pOS) const
{
  // TODO
}

void BM188X::Concat::accept(ComputeVisitor& pVisitor)
{
  pVisitor.visit(*this);
}

void BM188X::Concat::accept(ComputeVisitor& pVisitor) const
{
  pVisitor.visit(*this);
}
