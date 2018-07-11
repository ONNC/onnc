//===- ONNCJITApp.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_JUST_IN_TIME_INTERPRETER_APPLICATION_H
#define ONNC_JUST_IN_TIME_INTERPRETER_APPLICATION_H
#include <onnc/Core/Application.h>
#include "ONNCJITConfig.h"

class ONNCJITApp : public onnc::CoreApplication
{
public:
  ONNCJITApp(int pArgc, char* pArgv[]);

  ~ONNCJITApp();

  ONNCJITConfig& options() { return m_Options; }

  const ONNCJITConfig& options() const { return m_Options; }

  int run();

private:
  ONNCJITConfig m_Options;
};

#endif
