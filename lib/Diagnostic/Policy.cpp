//===- DiagnosticPolicy.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Diagnostic/Policy.h>
#include <onnc/Diagnostic/Diagnostic.h>
#include <onnc/Diagnostic/GeneralOptions.h>

using namespace onnc;
using namespace onnc::diagnostic;

//===----------------------------------------------------------------------===//
// DiagnosticPolicy
//===----------------------------------------------------------------------===//
bool DefaultPolicy::process(const GeneralOptions& pOptions, Diagnostic& pDiag)
{
  // if --fatal-warnings is turned on, then switch warnings and errors to fatal.
  if (pOptions.isFatalWarnings()) {
    if (Warning == pDiag.severity() || Error == pDiag.severity())
      pDiag.setSeverity(Fatal);
  }

  return true;
}
