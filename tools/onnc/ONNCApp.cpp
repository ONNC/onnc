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
#include <onnc/Target/TargetRegistry.h>
#include <onnc/Target/TargetBackend.h>
#include <onnc/Target/TargetOptions.h>
#include <onnc/IRReader/ONNXReader.h>
#include <onnc/IR/Module.h>
#include <onnc/Core/PassManager.h>
#include <onnc/ADT/Color.h>
#include <onnc/Support/IOStream.h>
#include <string>

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
  onnc::onnx::Reader reader;
  Module module;
  if (!reader.parse(options().input(), module)) {
    return EXIT_FAILURE;
  }

  std::string error;
  std::string quadruple;
  options().quadruple().canonical(quadruple);
  const onnc::Target* target = TargetRegistry::Lookup(quadruple, error);
  if (nullptr == target) {
    errs() << Color::RED << "Error" << Color::RESET
           << ": can not found target `" << quadruple << "`: " << error
           << std::endl;
    return EXIT_FAILURE;
  }

  PassManager pm;
  TargetBackend* backend = target->createBackend(options().target());
  backend->addTensorSel(pm);
  backend->addMemAlloc(pm);
  backend->addCodeEmit(pm, options().output());

  pm.run(module);

  return EXIT_SUCCESS;
}
