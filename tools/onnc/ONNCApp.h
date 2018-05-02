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
#include "ONNCConfig.h"

class ONNCApp : public onnc::CoreApplication
{
public:
  ONNCApp(int pArgc, char* pArgv[]);

  ~ONNCApp();

  ONNCConfig& options() { return m_Options; }

  const ONNCConfig& options() const { return m_Options; }

  int compile();

private:
  ONNCConfig m_Options;
};

#endif
