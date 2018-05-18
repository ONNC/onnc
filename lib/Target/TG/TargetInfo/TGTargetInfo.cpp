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

unsigned int TG1680MatchFn(const Quadruple &pQuadruple) {
  unsigned int score = 0;
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
  return score;
}

unsigned int TG1682MatchFn(const Quadruple &pQuadruple) {
  unsigned int score = 0;
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
  if (Quadruple::SophonSubArch_vBM1682 == pQuadruple.getSubArch()){
    score += 30;
  }
  return score;
}

unsigned int TG1880MatchFn(const Quadruple &pQuadruple) {
  unsigned int score = 0;
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
  if (Quadruple::SophonSubArch_vBM1880 == pQuadruple.getSubArch()){
    score += 30;
  }
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

onnc::Target& onnc::getTheTGBM1880Target()
{
  static onnc::Target tgbm1880_target;
  return tgbm1880_target;
}

extern "C" void InitializeTGONNCPlatform() {
  onnc::TargetRegistry::RegisterTarget(getTheTGBM1880Target(),
      "TGBM1880", "BM1880Backend on TG", TG1880MatchFn);
  onnc::TargetRegistry::RegisterTarget(getTheTGBM1680Target(),
      "TGBM1680", "BM1680Backend on TG", TG1680MatchFn);
  onnc::TargetRegistry::RegisterTarget(getTheTGBM1682Target(),
      "TGBM1682", "BM1682Backend on TG", TG1682MatchFn);
}
