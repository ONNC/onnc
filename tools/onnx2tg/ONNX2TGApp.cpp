//===- ONNX2TG.cpp --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNX2TGApp.h"
#include <cstdlib>
#include <onnc/Target/TargetSelect.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ONNX2TG
//===----------------------------------------------------------------------===//
ONNX2TG::ONNX2TG(int pArgc, char* pArgv[])
  : onnc::CoreApplication(pArgc, pArgv) {
  InitializeAllPlatforms();
  InitializeAllBackends();
}

ONNX2TG::~ONNX2TG()
{
}

int ONNX2TG::compile()
{
  // TODO
  return EXIT_SUCCESS;
}
