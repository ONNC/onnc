//===- StreamLog.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_DIAGNOSTIC_STREAM_LOG_H
#define ONNC_DIAGNOSTIC_STREAM_LOG_H
#include <onnc/Support/OStream.h>
#include <onnc/Diagnostic/Logger.h>

namespace onnc {
namespace diagnostic {

class Diagnostic;
class GeneralOptions;

/** \class StreamLog
 *  \brief StreamLog display the result of diagnostic in text
 *  format.
 */
class StreamLog : public Logger
{
public:
  explicit StreamLog(OStream& pOStream);

  /// Start logging
  virtual void start() { /** do nothing **/ }

  /// stop logging
  virtual void stop() { /** do nothing **/ }

private:
  void unreachable(const std::string& pMessage);

  void fatal(const std::string& pMessage);

  void error(const std::string& pMessage);

  void warning(const std::string& pMessage);

  void debug(const std::string& pMessage);

  void note(const std::string& pMessage);

  void ignore(const std::string& pMessage);

  void printTooManyErrors();

  void printTooManyWarnings();

private:
  OStream& m_OStream;
};

} // namespace of diagnostic
} // namespace of onnc

#endif
