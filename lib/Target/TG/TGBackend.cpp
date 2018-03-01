//===- TGBackend.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "TGBackend.h"
#include "TargetInfo/TGTargetInfo.h"
#include <onnc/Target/TargetRegistry.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// TGBackend
//===----------------------------------------------------------------------===//
TargetBackend* CreateTGBM1680Backend(const CompilerConfig& pConfig)
{
  // TODO
  return nullptr;
}

TargetBackend* CreateTGBM1682Backend(const CompilerConfig& pConfig)
{
  // TODO
  return nullptr;
}

extern "C" void InitializeTGONNCBackend()
{
  onnc::TargetRegistry::RegisterTargetBackend(getTheTGBM1680Target(),
      CreateTGBM1680Backend);
  onnc::TargetRegistry::RegisterTargetBackend(getTheTGBM1682Target(),
      CreateTGBM1682Backend);
}
