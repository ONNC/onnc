//===- VanillaTargetInfo.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "VanillaTargetInfo.h"
#include <onnc/IR/Quadruple.h>
#include <onnc/Target/TargetRegistry.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//

  
static unsigned int VanillaMatchFn(const Quadruple& pQuadruple)
{
  unsigned int score = 0;
  if (Quadruple::vanilla == pQuadruple.getArch()) {
    score += 10;
  }

  return score;
}


Target& onnc::getTheVanillaTarget()
{
  static Target vanilla_target;
  return vanilla_target;
}


extern "C" void InitializeVanillaONNCPlatform() {
  onnc::TargetRegistry::RegisterTarget(onnc::getTheVanillaTarget(), "vanilla",
                                       "Vanilla DLA", VanillaMatchFn);
}
