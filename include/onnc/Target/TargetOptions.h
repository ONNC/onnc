//===- TargetOptions.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TARGET_OPTIONS_H
#define ONNC_TARGET_TARGET_OPTIONS_H

#include <string>
#include <utility>

#include <onnc/Support/Path.h>

namespace onnc {

/** \class TargetOptions
 *  \brief TargetOptions stores settings of a compiler
 */
class TargetOptions
{
public:
  enum class NvDlaConfigSet : unsigned {
    nv_small,
    nv_full,
  };

  enum class NvDlaExecutionMode : unsigned {
    direct,
    image,
  };

public:
  TargetOptions() = default;
  TargetOptions(const TargetOptions& pCopy) = default;

  TargetOptions& operator=(const TargetOptions& pCopy) = default;

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

  Path getResourceDirectory() const { return m_ResourceDirectory; }

  void setResourceDirectory(Path newResourceDirectory) {
    m_ResourceDirectory = std::move(newResourceDirectory);
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

  /// TODO: move to NvDlaBackend related files
  NvDlaConfigSet getNvDlaConfigSet() const noexcept { return m_NvDlaConfigSet; }

  /// TODO: move to NvDlaBackend related files
  void setNvDlaConfigSet(NvDlaConfigSet pConfigSet) noexcept { m_NvDlaConfigSet = pConfigSet; }

  /// TODO: move to NvDlaBackend related files
  NvDlaExecutionMode getNvDlaExecutionMode() const noexcept { return m_NvDlaExecutionMode; }

  /// TODO: move to NvDlaBackend related files
  void setNvDlaExecutionMode(NvDlaExecutionMode pExecutionMode) { m_NvDlaExecutionMode = pExecutionMode; }

  /// TODO: move to NvDlaBackend related files
  bool shouldEnableLayerFusion() const noexcept { return m_EnableLayerFusion; }

  /// TODO: move to NvDlaBackend related files
  void enableLayerFusion(bool pEnable = true) noexcept { m_EnableLayerFusion = pEnable; }

private:
  bool m_PrintModuleBeforeSel = false;
  bool m_IgnoreCalibrationStep = false;
  bool m_AddDummyCTable = false;
  bool m_AddDummyWeight = false;
  Path m_ResourceDirectory;
  std::string m_OptOnnxModel;

  /// TODO: move to NvDlaBackend related files
  NvDlaConfigSet m_NvDlaConfigSet = NvDlaConfigSet::nv_full;
  NvDlaExecutionMode m_NvDlaExecutionMode = NvDlaExecutionMode::direct;
  bool m_EnableLayerFusion = false;
};

} // namespace onnc

#endif
