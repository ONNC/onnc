//===- ReadONNXApp.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_READ_ONNX_APPLICATION_H
#define ONNC_READ_ONNX_APPLICATION_H
#include <onnc/Core/Application.h>
#include "ReadONNXConfig.h"

class ReadONNXApp : public onnc::CoreApplication
{
public:
  ReadONNXApp(int pArgc, char* pArgv[]);

  ~ReadONNXApp();

  ReadONNXConfig& options() { return m_Options; }

  const ReadONNXConfig& options() const { return m_Options; }

  int display();

private:
  ReadONNXConfig m_Options;
};

#endif
