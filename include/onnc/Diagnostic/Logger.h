//===- Logger.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_DIAGNOSTIC_LOGGER_H
#define ONNC_DIAGNOSTIC_LOGGER_H
#include <onnc/Support/OStream.h>

namespace onnc {
namespace diagnostic {

static const enum OStream::Colors UnreachableColor = OStream::MAGENTA;
static const enum OStream::Colors FatalColor       = OStream::MAGENTA;
static const enum OStream::Colors ErrorColor       = OStream::RED;
static const enum OStream::Colors WarningColor     = OStream::YELLOW;
static const enum OStream::Colors DebugColor       = OStream::CYAN;
static const enum OStream::Colors NoteColor        = OStream::GREEN;
static const enum OStream::Colors IgnoreColor      = OStream::BLUE;

class Diagnostic;

/** \class Logger
 *  \brief Logger display the result of diagnostic in text
 *  format.
 */
class Logger
{
public:
  /// @param[in] pMaxWarning Maximal warning messages
  /// @param[in] pMaxErrors  Maximal error messages
  Logger(unsigned int pMaxWarning, unsigned int pMaxErrors);

  virtual ~Logger() { }

  /// Start logging
  virtual void start() = 0;

  /// stop logging
  virtual void stop() = 0;

  virtual bool handle(const Diagnostic& pDiag);

  unsigned int getMaxWarnings() const { return m_MaxWarnings; }

  unsigned int getNumWarnings() const { return m_NumWarnings; }

  unsigned int getMaxErrors() const { return m_MaxErrors; }

  unsigned int getNumErrors() const { return m_NumErrors; }

protected:
  virtual void unreachable(const std::string& pMessage) = 0;

  virtual void fatal(const std::string& pMessage) = 0;

  virtual void error(const std::string& pMessage) = 0;

  virtual void warning(const std::string& pMessage) = 0;

  virtual void debug(const std::string& pMessage) = 0;

  virtual void note(const std::string& pMessage) = 0;

  virtual void ignore(const std::string& pMessage) = 0;

  virtual void printTooManyErrors() = 0;

  virtual void printTooManyWarnings() = 0;

protected:
  unsigned int m_MaxWarnings;
  unsigned int m_NumWarnings;
  unsigned int m_MaxErrors;
  unsigned int m_NumErrors;
};

} // namespace of diagnostic
} // namespace of onnc

#endif
