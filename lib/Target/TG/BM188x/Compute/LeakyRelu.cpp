//===- LeakyRelu.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "LeakyRelu.h"
#include "../BM188xVisitor.h"
#include <onnc/Support/Casting.h>

#define DEBUG_TYPE "tg_leakyrelu"

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// LeakyRelu
//===----------------------------------------------------------------------===//
BM188X::LeakyRelu::LeakyRelu(const FloatAttr &pAlpha)
    : onnc::LeakyRelu(pAlpha), m_GTRShiftWidth(0), m_LERShiftWidth(0),
      m_GTScale(0), m_LEScale(0)
{
}

BM188X::LeakyRelu::~LeakyRelu()
{
}

void BM188X::LeakyRelu::print(std::ostream &pOS) const
{
  // TODO
}

void BM188X::LeakyRelu::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::LeakyRelu::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}
