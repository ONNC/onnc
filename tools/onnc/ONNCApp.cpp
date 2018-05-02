//===- ONNCApp.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNCApp.h"
#include <cstdlib>
#include <onnc/Target/TargetSelect.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ONNCApp
//===----------------------------------------------------------------------===//
ONNCApp::ONNCApp(int pArgc, char* pArgv[])
  : onnc::CoreApplication(pArgc, pArgv),
    m_Options() {
  InitializeAllPlatforms();
  InitializeAllBackends();
}

ONNCApp::~ONNCApp()
{
}

int ONNCApp::compile()
{
  // TODO
  return EXIT_SUCCESS;
}
