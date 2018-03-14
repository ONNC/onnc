//===- main.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNX2TGApp.h"
#include <onnc/Option/CommandLine.h>
#include <onnc/Support/Path.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Main procedure
//===----------------------------------------------------------------------===//
int main(int pArgc, char* pArgv[])
{
  ONNX2TG onnx2tg(pArgc, pArgv);
  return onnx2tg.compile();
}
