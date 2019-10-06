//===- TargetOptions.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TARGET_OPTIONS_H
#define ONNC_TARGET_TARGET_OPTIONS_H

#include <onnc/Support/Path.h>

#include <string>
#include <utility>

namespace onnc {
namespace nvdla {

enum class ConfigSet : unsigned
{
  nv_full = 0,
};

enum class ExecutionMode : unsigned
{
  direct = 0,
};

} // namespace nvdla

enum class RunMode : unsigned
{
  normal = 0,
};

/** \class TargetOptions
 *  \brief TargetOptions stores settings of a compiler
 */
class TargetOptions
{
public:
  TargetOptions()                           = default;
  TargetOptions(const TargetOptions& pCopy) = default;

  TargetOptions& operator=(const TargetOptions& pCopy) = default;

  /// The property holds whether printing module before tensor selection
  bool shouldPrintBeforeTensorSel() const { return m_PrintModuleBeforeSel; }

  void printBeforeTensorSel(bool pEnable = true) { m_PrintModuleBeforeSel = pEnable; }

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

  /// This property holds the output file name for optimized onnx model
  /// in backend
  std::string optOnnxModel() const { return m_OptOnnxModel; }

  void optOnnxModel(std::string pFileName) { m_OptOnnxModel = pFileName; }

  unsigned getVerboseLevel() const noexcept { return m_VerboseLevel; }

  void setVerboseLevel(unsigned verboseLevel) noexcept { m_VerboseLevel = verboseLevel; }

private:
  bool        m_PrintModuleBeforeSel  = false;
  bool        m_IgnoreCalibrationStep = false;
  bool        m_AddDummyCTable        = false;
  bool        m_AddDummyWeight        = false;
  unsigned    m_VerboseLevel          = 0;
  std::string m_OptOnnxModel          = "";
};

} // namespace onnc

#endif
