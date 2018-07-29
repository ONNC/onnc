//===- Config.cpp --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Config.h"

//===----------------------------------------------------------------------===//
// Config
//===----------------------------------------------------------------------===//
Config::Config()
  : m_Input(), m_Output() {
}

onnc::TargetOptions &Config::target()
{
  return m_Options;
}

const onnc::TargetOptions &Config::target() const
{
  return m_Options;
}
