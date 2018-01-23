//===- MsgHandling.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/Support/ManagedStatic.h>
#include <onnc/ADT/OwningPtr.h>

using namespace onnc;

static onnc::ManagedStatic<diagnostic::Engine> g_pEngine;

//===----------------------------------------------------------------------===//
// Non-member Functions
//===----------------------------------------------------------------------===//
/// Diagnose - check system status and flush all error messages.
bool onnc::diagnostic::Diagnose()
{
  if (0 < diagnostic::getEngine().hasError())
    return false;
  return true;
}

/// getDiagnosticEngine - Get the global-wise diagnostic engine.
diagnostic::Engine& onnc::diagnostic::getEngine()
{
  return *g_pEngine;
}
