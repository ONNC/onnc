//===- ONNIApp.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNIApp.h"

#include "CountOperatorsPass.h"
#include "InterpreterPass.h"
#include "OnnxOptPass.h"

#include <onnc/ADT/Color.h>
#include <onnc/Config/ONNX.h>
#include <onnc/Target/TargetSelect.h>
#include <onnc/Target/TargetRegistry.h>
#include <onnc/Target/TargetBackend.h>
#include <onnc/Target/TargetOptions.h>
#include <onnc/IRReader/ONNXReader.h>
#include <onnc/IR/Module.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Core/PassManager.h>
#include <onnc/ADT/Color.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Analysis/GlobalStatistics.h>

#include <cstdlib>
#include <fstream>
#include <string>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ONNIApp
//===----------------------------------------------------------------------===//
ONNIApp::ONNIApp(int pArgc, char* pArgv[])
  : onnc::CoreApplication(pArgc, pArgv),
    m_Options() {
  InitializeAllPlatforms();
  InitializeAllBackends();
}

ONNIApp::~ONNIApp()
{
}

int ONNIApp::run()
{
  onnc::onnx::Reader reader;
  Module module;
  SystemError err = reader.parse(options().model(), module);
  if (!err.isGood()) {
    // TODO: show error message
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

  if (options().onnxOpt()) {
    pm.add(CreateOnnxOptPass());
  }

  TargetBackend* backend = target->createBackend(options().target());
  backend->addTensorSel(pm);
  backend->addTensorSched(pm);
  backend->addMemAlloc(pm);
  if (options().verbose() >= 3) {
    pm.add(CreateCountOperatorsPass("[Statistics] "));
  }

  // FIXME: Use onnc-runtime to handle input
  char *input_mem = NULL;
  if (!options().dryRun()) {
    if (!exists(options().input())) {
      errs() << Color::MAGENTA << "Fatal" << Color::RESET
             << ": input file not found: " << options().input()
             << std::endl;
      return EXIT_FAILURE;
    }
    if (!is_regular(options().input())) {
      errs() << Color::MAGENTA << "Fatal" << Color::RESET
             << ": input file is not a regular file: " << options().input()
             << std::endl;
      return EXIT_FAILURE;
    }

    xTensorProto tensor;
    std::ifstream input_fin(options().input().native());
    tensor.ParseFromIstream(&input_fin);
    const std::string &raw_data_str = tensor.raw_data();
    input_mem = new char[raw_data_str.size()];
    memcpy(input_mem, raw_data_str.data(), raw_data_str.size());
  }
  pm.add(CreateInterpreterPass(backend, input_mem,
                               options().verbose(), options().dryRun()));

  pm.run(module);

  if (options().verbose() >= 3) {
    errs() << "==== print CountOperatorsPass result again ====\n";
    StatisticsGroup group = global::stats()->group("CountOperatorsPass");
    StringList opList = group.entryList();
    for(auto listItr=opList.begin(); listItr != opList.end(); ++listItr){
      errs() << *listItr << ": " << group.readEntry(*listItr, 0) << std::endl;
    }
    errs() << "==== end again of printing CountOperatorsPass ====\n";
  }
  delete input_mem;
  return EXIT_SUCCESS;
}
