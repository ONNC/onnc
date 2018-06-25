//===- ONNX2TG.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_COMPILER_ONNX_TO_TG_APPLICATION_H
#define ONNC_COMPILER_ONNX_TO_TG_APPLICATION_H
#include "Config.h"
#include <onnc/Core/Application.h>

class ONNX2TG : public onnc::CoreApplication
{
public:
  ONNX2TG(int pArgc, char *pArgv[]);

  ~ONNX2TG() override;

  int compile();

  Config &options() { return m_Config; }

  const Config &options() const { return m_Config; }

private:
  Config m_Config;
};

#endif
