//===- ONNIApp.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_INTERPRETER_APPLICATION_H
#define ONNC_INTERPRETER_APPLICATION_H
#include <onnc/Core/Application.h>
#include "ONNIConfig.h"

class ONNIApp : public onnc::CoreApplication
{
public:
  ONNIApp(int pArgc, char* pArgv[]);

  ONNIConfig& options() { return m_Options; }

  const ONNIConfig& options() const { return m_Options; }

  int run();

private:
  ONNIConfig m_Options;
};

#endif
