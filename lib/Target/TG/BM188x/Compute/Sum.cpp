//===- Sum.cpp ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Sum.h"
#include "../BM188xVisitor.h"
#include <onnc/Support/Casting.h>

#define DEBUG_TYPE "tg_sum"

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// BM188X::Sum
//===----------------------------------------------------------------------===//
BM188X::Sum::Sum()
    :  m_RShiftWidth(0),  m_DoRelu(0), m_ThresholdXQuantized()
{
}

void BM188X::Sum::print(std::ostream& pOS) const
{
  // TODO
}

void BM188X::Sum::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::Sum::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}
