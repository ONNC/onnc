//===- SophonBackend.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "SophonBackend.h"
#include "TargetInfo/SophonTargetInfo.h"
#include <onnc/Target/TargetRegistry.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// SophonBackend
//===----------------------------------------------------------------------===//
TargetBackend* CreateBM1680Backend(const TargetOptions& pOptions)
{
  // TODO
  return nullptr;
}

TargetBackend* CreateBM1682Backend(const TargetOptions& pOptions)
{
  // TODO
  return nullptr;
}

extern "C" void InitializeSophonONNCBackend()
{
  onnc::TargetRegistry::RegisterTargetBackend(getTheBM1680Target(),
      CreateBM1680Backend);
  onnc::TargetRegistry::RegisterTargetBackend(getTheBM1682Target(),
      CreateBM1682Backend);
}
