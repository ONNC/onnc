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
void X86Backend::addTensorSel(PassManager& pPM)
{
  // TODO
}

void X86Backend::addMemAlloc(PassManager& pPM)
{
  // TODO
}

void X86Backend::addCodeEmit(PassManager& pPM, const Path& pOutput)
{
  // TODO
}

//===----------------------------------------------------------------------===//
// Non member functions
//===----------------------------------------------------------------------===//
TargetBackend* CreateX86_32Backend(const TargetOptions& pOptions)
{
  return new X86_32Backend(pOptions);
}

TargetBackend* CreateX86_64Backend(const TargetOptions& pOptions)
{
  return new X86_64Backend(pOptions);
}

extern "C" void InitializeX86ONNCBackend()
{
  onnc::TargetRegistry::RegisterTargetBackend(getTheX86_32Target(),
      CreateX86_32Backend);
  onnc::TargetRegistry::RegisterTargetBackend(getTheX86_64Target(),
      CreateX86_64Backend);
}
