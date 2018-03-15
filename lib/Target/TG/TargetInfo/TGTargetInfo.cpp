//===- TGTargetInfo.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "TGTargetInfo.h"
#include <onnc/Target/TargetRegistry.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

namespace onnc {

unsigned int TGQuadrupleMatchFn(const Quadruple &pQuadruple) {
  unsigned int score = 0;
  printf("getArchName=%s, getArchSubArch=%d\n", pQuadruple.getArchName().data(), pQuadruple.getSubArch());
  if (Quadruple::sophon == pQuadruple.getArch()) {
    score += 10;
  }
  if (Quadruple::BITMAIN == pQuadruple.getArchVendor()) {
    score += 10;
  }
  if (Quadruple::BMNet == pQuadruple.getEnvironment()) {
    score += 10;
  }
  if (Quadruple::SophonArchCore_TG == pQuadruple.getArchCore()) {
    score += 30;
  }
  if (Quadruple::SophonSubArch_vBM1680 == pQuadruple.getSubArch()){
    score += 30;
  }
  errs() << score << std::endl;
  return score;
}

}

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
  onnc::TargetRegistry::RegisterTarget(getTheTGBM1680Target(),
      "TGBM1680", "BM1680Backend on TG", TGQuadrupleMatchFn);
//  onnc::TargetRegistry::RegisterTarget(getTheTGBM1682Target(),
//      "TGBM1682", "BM16802ackend on TG", TGQuadrupleMatchFn);
}
