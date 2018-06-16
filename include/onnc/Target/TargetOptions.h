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
    TargetOptions()
        : PrintMachineCode(false), PrintModuleBeforeSel(false),
          IgnoreCalibrationStep(false)
    {
    }
    ~TargetOptions() {};

    /// PrintMachineCode - This flag is enabled when the -print-machineinstrs
    ///// option is specified on the command line, and should enable debugging
    ///// output from the code generator.
    unsigned PrintMachineCode : 1;

    // PrintModuleBeforeSel- print module before target lowering pass in backend
    unsigned PrintModuleBeforeSel : 1;

    // IgnoreCalibrationStep
    unsigned IgnoreCalibrationStep : 1;
};

} // namespace of onnc

#endif
