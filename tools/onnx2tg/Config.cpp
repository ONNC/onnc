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
Config::Config() : m_Input(), m_Output() {}
bool Config::PrintModuleBeforeISel() const
{
  return m_Options.PrintModuleBeforeSel;
}

void Config::setPrintModuleBeforeISel(bool pSet)
{
  m_Options.PrintModuleBeforeSel = pSet;
}

bool Config::DumpASM() const { return m_Options.DumpASM; }

void Config::setDumpASM(bool pSet) { m_Options.DumpASM = pSet; }

bool Config::IgnoreCalibrationStep() const
{
  return m_Options.IgnoreCalibrationStep;
}

void Config::setIgnoreCalibrationStep(bool pSet)
{
  m_Options.IgnoreCalibrationStep = pSet;
}
onnc::TargetOptions &Config::getOptions() { return m_Options; }
