//===-- TargetOptions.cpp -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Target/TargetOptions.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// TargetOptions
//===----------------------------------------------------------------------===//
TargetOptions::TargetOptions()
  : m_PrintModuleBeforeSel(false), m_IgnoreCalibrationStep(false),
    m_AddDummyCTable(false), m_AddDummyWeight(false) {
}

TargetOptions::TargetOptions(const TargetOptions& pCopy)
  : m_PrintModuleBeforeSel(pCopy.shouldPrintBeforeTensorSel()),
    m_IgnoreCalibrationStep(pCopy.shouldIgnoreCalibrationStep()),
    m_AddDummyCTable(pCopy.shouldUseDummyCTable()),
    m_AddDummyWeight(pCopy.shouldUseDummyWeight()) {
}

TargetOptions& TargetOptions::operator=(const TargetOptions& pCopy)
{
  m_PrintModuleBeforeSel = pCopy.shouldPrintBeforeTensorSel();
  m_IgnoreCalibrationStep = pCopy.shouldIgnoreCalibrationStep();
  m_AddDummyCTable = pCopy.shouldUseDummyCTable();
  m_AddDummyWeight = pCopy.shouldUseDummyWeight();
  return *this;
}
