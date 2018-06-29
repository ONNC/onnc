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
 *  XXX: should it be in Target/ or Core/?
 */
class TargetOptions
{
public:
  bool m_DumpASM{ false };

  // PrintModuleBeforeSel- print module before target lowering pass in backend
  bool m_PrintModuleBeforeSel{ false };

  // IgnoreCalibrationStep
  bool m_IgnoreCalibrationStep{ false };

  bool m_NoFuse{ false };
};

} // namespace onnc

#endif
