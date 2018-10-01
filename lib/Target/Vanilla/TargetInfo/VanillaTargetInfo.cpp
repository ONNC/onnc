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

///using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//

namespace onnc {
  
unsigned int VanillaMatchFn(const Quadruple& pQuadruple)
{
  unsigned int score = 0;
  if (Quadruple::vanilla == pQuadruple.getArch()) {
    score += 10;
  }

#if 0
  if (Quadruple::BITMAIN == pQuadruple.getArchVendor()) {
    score += 10;
  }
  if (Quadruple::BMNet == pQuadruple.getEnvironment()) {
    score += 10;
  }
  if (Quadruple::SophonArchCore_TG == pQuadruple.getArchCore()) {
    score += 30;
  }
  if (Quadruple::SophonSubArch_vBM1880 == pQuadruple.getSubArch()) {
    score += 30;
  }
#endif
  
  return score;
}


Target& getTheVanillaTarget()
{
  static Target vanilla_target;
  return vanilla_target;
}

} // namespace onnc


extern "C" void InitializeVanillaONNCPlatform() {
  onnc::TargetRegistry::RegisterTarget(onnc::getTheVanillaTarget(), "vanilla",
                                       "Vanilla DLA", onnc::VanillaMatchFn);
  
  //onnc::RegisterTarget<onnc::Quadruple::vanilla>(onnc::getTheVanillaTarget(),
  //    "vanilla", "Vanilla DLA");
}
