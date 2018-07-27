//===- Conv.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#define DEBUG_TYPE "tg_conv"
#include "Conv.h"

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// Conv
//===----------------------------------------------------------------------===//
BM188X::Conv::Conv()
    : onnc::Conv(),  m_DoBias(0), /* m_DoRelu(0),*/ m_DoScale(0),
      m_DoScaleBias(0), m_RShiftWidth(0), m_ScaleRShiftWidth(0),
      m_ConvOutputThreshold(0.f)
{
}

BM188X::Conv::~Conv()
{
}

void BM188X::Conv::print(std::ostream& pOS) const
{
  // TODO
}