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
  return m_Options.m_PrintModuleBeforeSel;
}

void Config::setPrintModuleBeforeISel(bool pSet)
{
  m_Options.m_PrintModuleBeforeSel = pSet;
}

bool Config::DumpASM() const { return m_Options.m_DumpASM; }

void Config::setDumpASM(bool pSet) { m_Options.m_DumpASM = pSet; }

bool Config::IgnoreCalibrationStep() const
{
  return m_Options.m_IgnoreCalibrationStep;
}

void Config::setIgnoreCalibrationStep(bool pSet)
{
  m_Options.m_IgnoreCalibrationStep = pSet;
}

bool Config::AddDummyCTable() const { return m_Options.m_AddDummyCTable; }
void Config::setAddDummyCTable(bool pSet) { m_Options.m_AddDummyCTable = pSet; }
onnc::TargetOptions &Config::getOptions() { return m_Options; }
