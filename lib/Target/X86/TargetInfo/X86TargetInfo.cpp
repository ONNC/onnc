//===- X86TargetInfo.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "X86TargetInfo.h"

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
onnc::Target& onnc::getTheX86_32Target()
{
  static onnc::Target x86_32_target;
  return x86_32_target;
}

onnc::Target& onnc::getTheX86_64Target()
{
  static onnc::Target x86_64_target;
  return x86_64_target;
}

extern "C" void InitializeX86ONNCPlatform() {
}
