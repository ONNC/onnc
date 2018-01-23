//===- DiagnosticEngine.tcc -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_DIAGNOSTIC_DIAGNOSTIC_ENGINE_TCC
#define ONNC_DIAGNOSTIC_DIAGNOSTIC_ENGINE_TCC
#include <assert.h>

namespace onnc {
namespace diagnostic {

/// Emit the message
template<typename PolicyType>
bool Engine::emit()
{
  Diagnostic info(m_State, m_InfoMap);

  // Change the severity. If --fatal-warnings turns on, then all warnings
  // is changed as fatal errors.
  bool emitted = PolicyType::process(m_Options, info);

  // Print out the message in Diagnostic.
  m_pLogger->handle(info);
  return emitted;
}

} // namespace of diagnostic
} // namespace of onnc
#endif
