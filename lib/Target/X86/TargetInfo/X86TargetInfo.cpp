//===- X86TargetInfo.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "X86TargetInfo.h"
#include <onnc/IR/Quadruple.h>
#include <onnc/Target/TargetRegistry.h>

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
  RegisterTarget<Quadruple::x86>(getTheX86_32Target(),
      "x86", "32-bit X86: Pentium-Pro and above");

  RegisterTarget<Quadruple::x86_64>(getTheX86_64Target(),
      "x86-64", "64-bit X86: EM64T and AMD64");
}
