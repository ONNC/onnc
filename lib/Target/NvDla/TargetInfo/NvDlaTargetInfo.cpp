//===- NvDlaTargetInfo.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaTargetInfo.h"

#include <onnc/IR/Quadruple.h>
#include <onnc/Target/TargetRegistry.h>

using namespace onnc;

static unsigned int NvDlaMatchFn(const Quadruple& pQuadruple)
{
  unsigned int score = 0;
  if (Quadruple::nvdla == pQuadruple.getArch()) {
    score += 10;
  }
  return score;
}

Target& onnc::getTheNvDlaTarget()
{
  static Target nvdla_target;
  return nvdla_target;
}

extern "C" void InitializeNvDlaONNCPlatform()
{
  onnc::TargetRegistry::RegisterTarget(onnc::getTheNvDlaTarget(), "nvdla", "NvDla DLA", NvDlaMatchFn);
}
