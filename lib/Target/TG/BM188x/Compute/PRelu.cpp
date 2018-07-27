//===- PRelu.cpp ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#define DEBUG_TYPE "tg_prelu"
#include "PRelu.h"

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// PRelu
//===----------------------------------------------------------------------===//
BM188X::PRelu::PRelu()
    : onnc::PRelu(), m_GTRShiftWidth(0), m_LERShiftWidth(0), m_GTScale(0)
{
}

BM188X::PRelu::~PRelu()
{
}

void BM188X::PRelu::print(std::ostream &pOS) const
{
  // TODO
}