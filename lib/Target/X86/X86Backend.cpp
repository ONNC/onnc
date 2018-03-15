//===- X86Backend.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "X86Backend.h"
#include "TargetInfo/X86TargetInfo.h"
#include <onnc/Target/TargetRegistry.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// X86Backend
//===----------------------------------------------------------------------===//
TargetBackend* CreateX86_32Backend(const TargetOptions& pOptions)
{
  // TODO
  return nullptr;
}

TargetBackend* CreateX86_64Backend(const TargetOptions& pOptions)
{
  // TODO
  return nullptr;
}

extern "C" void InitializeX86ONNCBackend()
{
  onnc::TargetRegistry::RegisterTargetBackend(getTheX86_32Target(),
      CreateX86_32Backend);
  onnc::TargetRegistry::RegisterTargetBackend(getTheX86_64Target(),
      CreateX86_64Backend);
}
