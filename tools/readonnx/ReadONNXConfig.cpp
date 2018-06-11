//===- ReadONNXConfig.cpp -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ReadONNXConfig.h"

using namespace onnc;

//===----------------------------------------------------------------------===//
// ReadONNXConfig
//===----------------------------------------------------------------------===//
ReadONNXConfig::ReadONNXConfig()
  : m_Input(), m_OStream(STDOUT_FILENO) {
}

ReadONNXConfig::~ReadONNXConfig()
{
  m_OStream.close();
}

void ReadONNXConfig::setOutput(const onnc::Path& pFileName)
{
  m_OStream.close();
  m_OStream.open(pFileName);
}
