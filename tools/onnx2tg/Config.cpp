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

const std::string &Config::ASMOutput() const { return m_Options.m_ASMOutput; }
void Config::setASMOutput(const std::string &pASMOutput)
{
  m_Options.m_ASMOutput = pASMOutput;
}

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

bool Config::AddDummyWeight() const { return m_Options.m_AddDummyWeight; }
void Config::setAddDummyWeight(bool pSet) { m_Options.m_AddDummyWeight = pSet; }

onnc::TargetOptions &Config::getOptions() { return m_Options; }
