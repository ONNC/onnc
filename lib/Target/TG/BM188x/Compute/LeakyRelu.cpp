//===- LeakyRelu.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#define DEBUG_TYPE "tg_leakyrelu"
#include "LeakyRelu.h"

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