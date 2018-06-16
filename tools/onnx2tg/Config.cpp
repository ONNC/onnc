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
    : m_Input(), m_Output(), m_PrintModuleBeforeISel(false),
      m_PrintMachineCode(false), m_IgnoreCalibrationStep(false)
{
}
