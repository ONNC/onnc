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
#include <onnc/Transforms/removeUnusedNodes.h>
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Transforms/PreTensorSel.h>
#include <onnc/Transforms/TensorSel.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// X86Backend
//===----------------------------------------------------------------------===//
X86Backend::X86Backend(const TargetOptions& pOptions)
  : NPUTargetBackend(pOptions) {
}

X86Backend::~X86Backend()
{
}

void X86Backend::addTensorSel(PassManager& pPM)
{
  // target independent pass
  pPM.add(createRemoveUnusedNodesPass());
  pPM.add(CreateUpdateGraphOutputSizePass());
  pPM.add(CreatePreTensorSel());
  pPM.add(CreateTensorSel(this));
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
// X86_32Backend
//===----------------------------------------------------------------------===//
X86_32Backend::X86_32Backend(const TargetOptions& pOptions)
  : X86Backend(pOptions) {
}

X86_32Backend::~X86_32Backend()
{
}

//===----------------------------------------------------------------------===//
// X86_64Backend
//===----------------------------------------------------------------------===//
X86_64Backend::X86_64Backend(const TargetOptions& pOptions)
  : X86Backend(pOptions) {
}

X86_64Backend::~X86_64Backend()
{
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
