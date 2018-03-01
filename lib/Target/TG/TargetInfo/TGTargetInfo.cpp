//===- TGTargetInfo.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "TGTargetInfo.h"

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
onnc::Target& onnc::getTheTGBM1680Target()
{
  static onnc::Target tgbm1680_target;
  return tgbm1680_target;
}

onnc::Target& onnc::getTheTGBM1682Target()
{
  static onnc::Target tgbm1682_target;
  return tgbm1682_target;
}

extern "C" void InitializeTGONNCPlatform() {
}
