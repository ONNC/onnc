//===- DiagnosticEngine.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_DIAGNOSTIC_ENGINE_H
#define ONNC_DIAGNOSTIC_ENGINE_H
#include <onnc/Diagnostic/DiagnosticInfoMap.h>
#include <onnc/Diagnostic/EngineFwd.h>
#include <onnc/Diagnostic/GeneralOptions.h>
#include <onnc/Diagnostic/Logger.h>
#include <onnc/Diagnostic/Policy.h>
#include <onnc/Diagnostic/Diagnostic.h>
#include <onnc/Support/DataTypes.h>
#include <onnc/ADT/StringRef.h>
#include <string>

namespace onnc {
namespace diagnostic {

class MsgHandler;

/** \class Engine
 *  \brief Engine drives the diagnostic system to display information.
 */
class Engine
{
public:
  Engine();

  ~Engine();

  /// reset logger
  void delegate(Logger* pLogger);

  /// Report a predefined diagnostic result
  MsgHandler report(unsigned int pID, Severity pSeverity);

  /// Report an arbitrary diagnostic result
  /// @param pMessage  [in] The formatted string of the report.
  /// @param pSeverity [in] The severity of the message.
  MsgHandler report(StringRef pMessage, Severity pSeverity);

  /// hasError - return true if the system has already encountered an error.
  bool hasError() const;

  /// Emit the message
  template<typename PolicyType = DefaultPolicy>
  bool emit();

 
  const GeneralOptions& settings() const { return m_Options; }

  GeneralOptions&       settings()       { return m_Options; }

private:
  friend class MsgHandler;
  friend class Diagnostic;

  const State& state() const  { return m_State; }
  State&       state()        { return m_State; }

private:
  GeneralOptions m_Options;
  Logger* m_pLogger;
  State m_State;
  InfoMap m_InfoMap;
};

} // namespace of diagnostic
} // namespace of onnc

#include "Bits/Engine.tcc"

#endif
