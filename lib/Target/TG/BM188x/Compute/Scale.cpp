//===- Scale.cpp ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Scale.h"
#include "../BM188xVisitor.h"
#include <onnc/Support/Casting.h>

#define DEBUG_TYPE "tg_scale"

using namespace onnc;

//===----------------------------------------------------------------------===//
// BM188X::Scale
//===----------------------------------------------------------------------===//
BM188X::Scale::Scale()
    : m_RShiftWidth(0)
{
}

void BM188X::Scale::print(std::ostream& pOS) const
{
  // TODO
}

void BM188X::Scale::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::Scale::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

