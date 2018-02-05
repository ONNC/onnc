//===- SophonTargetInfo.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "SophonTargetInfo.h"

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
onnc::Target& onnc::getTheBM1680Target()
{
  static onnc::Target bm1680_target;
  return bm1680_target;
}

onnc::Target& onnc::getTheBM1682Target()
{
  static onnc::Target bm1682_target;
  return bm1682_target;
}

extern "C" void InitializeSophonONNCPlatform() {
}
