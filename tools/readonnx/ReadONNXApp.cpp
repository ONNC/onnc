//===- ReadONNXApp.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ReadONNXApp.h"
#include <cstdlib>
#include <onnc/IRReader/ONNXReader.h>
#include <onnc/IR/Module.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/ADT/Color.h>
#include <onnc/Support/IOStream.h>
#include <string>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReadONNXApp
//===----------------------------------------------------------------------===//
ReadONNXApp::ReadONNXApp(int pArgc, char* pArgv[])
  : onnc::CoreApplication(pArgc, pArgv),
    m_Options() {
}

ReadONNXApp::~ReadONNXApp()
{
}

int ReadONNXApp::display()
{
  onnc::onnx::Reader reader;
  Module module;
  SystemError err = reader.parse(options().input(), module);
  if (!err.isGood()) {
    return EXIT_FAILURE;
  }
  module.print(options().output());
  return EXIT_SUCCESS;
}
