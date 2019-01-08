//===- NvdlaTargetInfo.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvdlaTargetInfo.h"
#include <onnc/IR/Quadruple.h>
#include <onnc/Target/TargetRegistry.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//

  
static unsigned int NvdlaMatchFn(const Quadruple& pQuadruple)
{
  unsigned int score = 0;
  if (Quadruple::nvdla == pQuadruple.getArch()) {
    score += 10;
  }

  return score;
}


Target& onnc::getTheNvdlaTarget()
{
  static Target nvdla_target;
  return nvdla_target;
}


extern "C" void InitializeNvdlaONNCPlatform() {
  onnc::TargetRegistry::RegisterTarget(onnc::getTheNvdlaTarget(), "nvdla",
                                       "Nvdla DLA", NvdlaMatchFn);
}
