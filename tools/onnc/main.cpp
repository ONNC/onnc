//===- main.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ONNCApp.h"

int main(int pArgc, char* pArgv[])
{
  ONNCApp onnc(pArgc, pArgv);
  return onnc.compile();
}
