//===- Sum.cpp ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#define DEBUG_TYPE "tg_sum"
#include "Sum.h"

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// BM188X::Sum
//===----------------------------------------------------------------------===//
BM188X::Sum::Sum()
    :  m_RShiftWidth(0),  m_DoRelu(0), m_ThresholdXQuantized()
{
}
