//===- CortexMTargetInfo.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "CortexMTargetInfo.h"
#include <onnc/IR/Quadruple.h>
#include <onnc/Target/TargetRegistry.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//

  
static unsigned int CortexMMatchFn(const Quadruple& pQuadruple)
{
  unsigned int score = 0;
  if (Quadruple::cortexm == pQuadruple.getArch()) {
    score += 10;
  }

  return score;
}


Target& onnc::getTheCortexMTarget()
{
  static Target cortexm_target;
  return cortexm_target;
}


extern "C" void InitializeCortexMONNCPlatform() {
  onnc::TargetRegistry::RegisterTarget(onnc::getTheCortexMTarget(), "cortexm",
                                       "CortexM DLA", CortexMMatchFn);
}
