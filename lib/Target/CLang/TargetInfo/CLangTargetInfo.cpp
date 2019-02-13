//===- CLangTargetInfo.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "CLangTargetInfo.h"
#include <onnc/IR/Quadruple.h>
#include <onnc/Target/TargetRegistry.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//

  
static unsigned int CLangMatchFn(const Quadruple& pQuadruple)
{
  unsigned int score = 0;
  if (Quadruple::clang == pQuadruple.getArch()) {
    score += 10;
  }

  return score;
}


Target& onnc::getTheCLangTarget()
{
  static Target clang_target;
  return clang_target;
}


extern "C" void InitializeCLangONNCPlatform() {
  onnc::TargetRegistry::RegisterTarget(onnc::getTheCLangTarget(), "clang",
                                       "CLang DLA", CLangMatchFn);
}
