//===- NVDLABackend.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NVDLABackend.h"

using namespace onnc;

//===----------------------------------------------------------------------===//
// NVDLABackend
//===----------------------------------------------------------------------===//
NVDLABackend::NVDLABackend(const TargetOptions& pOptions)
  : DLATargetBackend(pOptions) {
}

NVDLABackend::~NVDLABackend()
{
}

void NVDLABackend::addTensorSel(PassManager& pPM)
{
}

void NVDLABackend::addMemAlloc(PassManager& pPM)
{
}

void NVDLABackend::addCodeEmit(PassManager& pPM, const Path& pOutput)
{
}

void NVDLABackend::RegisterLowers(LowerRegistry& pRegistry) const
{
}

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
extern "C" void InitializeNVDLAONNCBackend()
{
}

extern "C" void InitializeNVDLAONNCPlatform()
{
}
