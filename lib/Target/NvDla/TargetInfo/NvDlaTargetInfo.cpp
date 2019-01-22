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

namespace {

template<Quadruple::SubArchType SubArch>
unsigned int NvDlaMatchFn(const Quadruple& pQuadruple)
{
  unsigned int score = 0;
  if (Quadruple::nvdla == pQuadruple.getArch()) {
    score += 10;
    if (SubArch == pQuadruple.getSubArch())
      score += 10;
  }
  return score;
}

} // anonymous namespace

Target& onnc::getFp16NvDlaTarget()
{
  static Target nvdla_target;
  return nvdla_target;
}

//===----------------------------------------------------------------------===//
// Initialization Function.
//===----------------------------------------------------------------------===//
extern "C" void InitializeNvDlaONNCPlatform() {
  onnc::TargetRegistry::RegisterTarget(
      onnc::getFp16NvDlaTarget(),
      "nvdla fp16",
      "NvDla Fp16",
      NvDlaMatchFn<Quadruple::NvDlaSubArch_fp16>);
}
