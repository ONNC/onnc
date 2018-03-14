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
#include <onnc/IRReader/ONNXReader.h>
#include <onnc/IR/Module.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ONNX2TG
//===----------------------------------------------------------------------===//
ONNX2TG::ONNX2TG(int pArgc, char* pArgv[])
  : onnc::CoreApplication(pArgc, pArgv), m_Config() {
  InitializeAllPlatforms();
  InitializeAllBackends();
}

ONNX2TG::~ONNX2TG()
{
}

int ONNX2TG::compile()
{
  onnc::onnx::Reader reader;
  Module module;
  if (!reader.parse(m_Config.input(), module)) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
