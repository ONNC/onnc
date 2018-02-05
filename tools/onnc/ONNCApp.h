//===- ONNCApp.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_COMPILER_APPLICATION_H
#define ONNC_COMPILER_APPLICATION_H
#include <onnc/Core/Application.h>

class ONNCApp : public onnc::CoreApplication
{
public:
  ONNCApp(int pArgc, char* pArgv[]);

  ~ONNCApp();

  int compile();
};

#endif
