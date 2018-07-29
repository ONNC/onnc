//===- TargetOptions.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_TARGET_OPTIONS_H
#define ONNC_CORE_TARGET_OPTIONS_H

namespace onnc {

/** \class TargetOptions
 *  \brief TargetOptions stores settings of a compiler
 */
class TargetOptions
{
public:
  TargetOptions();

  TargetOptions(const TargetOptions& pCopy);

  TargetOptions& operator=(const TargetOptions& pCopy);

  ~TargetOptions() { }

  /// The property holds whether printing module before tensor selection
  bool shouldPrintBeforeTensorSel() const { return m_PrintModuleBeforeSel; }

  void printBeforeTensorSel(bool pEnable = true) {
    m_PrintModuleBeforeSel = pEnable;
  }

  /// This property holds whether ignoring calibration step
  bool shouldIgnoreCalibrationStep() const { return m_IgnoreCalibrationStep; }

  void ignoreCalibrationStep(bool pEnable = true) {
    m_IgnoreCalibrationStep = pEnable;
  }

  /// This property holds whether adding dummy CTable
  bool shouldUseDummyCTable() const { return m_AddDummyCTable; }

  void useDummyCTable(bool pEnable = true) { m_AddDummyCTable = pEnable; }

  /// This property holds whether adding dummy weight
  bool shouldUseDummyWeight() const { return m_AddDummyWeight; }

  void useDummyWeight(bool pEnable = true) { m_AddDummyWeight = pEnable; }

private:
  bool m_PrintModuleBeforeSel;
  bool m_IgnoreCalibrationStep;
  bool m_AddDummyCTable;
  bool m_AddDummyWeight;
};

} // namespace onnc

#endif
