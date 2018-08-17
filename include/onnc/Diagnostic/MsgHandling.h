//===- MsgHandling.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_DIAGNOSTIC_MSG_HANDLING_H
#define ONNC_DIAGNOSTIC_MSG_HANDLING_H
#include <onnc/ADT/OwningPtr.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/Diagnostic/Engine.h>
#include <onnc/Diagnostic/MsgHandler.h>

namespace onnc {

/// Program should not pass though this code piece.
diagnostic::MsgHandler unreachable(unsigned int pID);

/// A fatal error. The program should stop immediately.
diagnostic::MsgHandler fatal      (unsigned int pID);

/// A normal error. The program tries to run as far as possible.
diagnostic::MsgHandler error      (unsigned int pID);

/// A warning. The result of the program is correct.
diagnostic::MsgHandler warning    (unsigned int pID);

/// A message for debugging.
diagnostic::MsgHandler debug      (unsigned int pID);

/// A message for noting.
diagnostic::MsgHandler note       (unsigned int pID);

/// A message that can be ignored in most cases.
diagnostic::MsgHandler ignore     (unsigned int pID);

diagnostic::MsgHandler unreachable(StringRef pMessage);
diagnostic::MsgHandler fatal      (StringRef pMessage);
diagnostic::MsgHandler error      (StringRef pMessage);
diagnostic::MsgHandler warning    (StringRef pMessage);
diagnostic::MsgHandler debug      (StringRef pMessage);
diagnostic::MsgHandler note       (StringRef pMessage);
diagnostic::MsgHandler ignore     (StringRef pMessage);

/** \namespace onnc::diagnostic
 *  \brief classes of diagnostic system
 *
 *  \b diagnostic namespace is the namespace for diagnostic system.
 *  The most frequent used library of the diagnostic system is
 *  @ref MsgHandling.h. It provides error-report functions to show
 *  error messages with appropriate severity. For example:
 *
 *  \code
 *  #include <onnc/Diagnostic/MsgHandling.h>
 *
 *  using namespace onnc;
 *  // show "Error parsing JSON file: file.json"
 *  fatal(fatal_json_parsing) << "file.json";
 *  \endcode
 *
 *  @ref fatal reads a message ID and print out formatted error message.
 *
 *  There are seven error severities of report functions:
 *  - @ref unreachable, the program should not run though these code pieces
 *  - @ref fatal, a fatal error. The program stops immediately.
 *  - @ref error, a normal error. The program keeps running as far as possible.
 *  - @ref warning, a warning. The program keeps running and the result is correct.
 *  - @ref debug, a message for debugging.
 *  - @ref note, a message for noting
 *  - @ref ignore, a message that can be ignored in most cases.
 */
namespace diagnostic {

/// diagnostic::getEngine - Get the global-wise diagnostic engine.
Engine& getEngine();

/// Diagnose - check system status and flush all error messages.
bool Diagnose();

} // namespace of diagnostic

//===----------------------------------------------------------------------===//
// Inline non-member function
//===----------------------------------------------------------------------===//
inline diagnostic::MsgHandler unreachable(unsigned int pID)
{
  return diagnostic::getEngine().report(pID, diagnostic::Unreachable);
}

inline diagnostic::MsgHandler fatal(unsigned int pID)
{
  return diagnostic::getEngine().report(pID, diagnostic::Fatal);
}

inline diagnostic::MsgHandler error(unsigned int pID)
{
  return diagnostic::getEngine().report(pID, diagnostic::Error);
}

inline diagnostic::MsgHandler warning(unsigned int pID)
{
  return diagnostic::getEngine().report(pID, diagnostic::Warning);
}

inline diagnostic::MsgHandler debug(unsigned int pID)
{
  return diagnostic::getEngine().report(pID, diagnostic::Debug);
}

inline diagnostic::MsgHandler note(unsigned int pID)
{
  return diagnostic::getEngine().report(pID, diagnostic::Note);
}

inline diagnostic::MsgHandler ignore(unsigned int pID)
{
  return diagnostic::getEngine().report(pID, diagnostic::Ignore);
}

inline diagnostic::MsgHandler unreachable(StringRef pMessage)
{
  return diagnostic::getEngine().report(pMessage, diagnostic::Unreachable);
}

inline diagnostic::MsgHandler fatal(StringRef pMessage)
{
  return diagnostic::getEngine().report(pMessage, diagnostic::Fatal);
}

inline diagnostic::MsgHandler error(StringRef pMessage)
{
  return diagnostic::getEngine().report(pMessage, diagnostic::Error);
}

inline diagnostic::MsgHandler warning(StringRef pMessage)
{
  return diagnostic::getEngine().report(pMessage, diagnostic::Warning);
}

inline diagnostic::MsgHandler debug(StringRef pMessage)
{
  return diagnostic::getEngine().report(pMessage, diagnostic::Debug);
}

inline diagnostic::MsgHandler note(StringRef pMessage)
{
  return diagnostic::getEngine().report(pMessage, diagnostic::Note);
}

inline diagnostic::MsgHandler ignore(StringRef pMessage)
{
  return diagnostic::getEngine().report(pMessage, diagnostic::Ignore);
}

} // namespace of onnc

#endif
