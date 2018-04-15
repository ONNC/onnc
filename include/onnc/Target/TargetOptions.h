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
    TargetOptions() : PrintMachineCode(false) {}
    ~TargetOptions() {};

    /// PrintMachineCode - This flag is enabled when the -print-machineinstrs
    ///// option is specified on the command line, and should enable debugging
    ///// output from the code generator.
    unsigned PrintMachineCode : 1;
};

} // namespace of onnc

#endif
