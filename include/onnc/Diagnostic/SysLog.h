//===- SysLog.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_DIAGNOSTIC_SYSLOG_H
#define ONNC_DIAGNOSTIC_SYSLOG_H
#include <onnc/Diagnostic/Logger.h>

namespace onnc {
namespace diagnostic {

/** \class SysLog
 *  \brief controls system log
 */
class SysLog : public Logger
{
public:
  class Settings
  {
  public:
    Settings();

    ~Settings();

    void setIndent(const std::string& pIndent) { m_Indent = pIndent; }

    const std::string& indent() const { return m_Indent; }

    /// If true, the the message will write directly to system console if there
    /// is an error while sending to system logger.
    bool isLogConsole() const;

    void setLogConsole(bool pEnable = true);

    /// If true, open the connection immediately.
    bool isNotDelay() const;

    void setNotDelay(bool pEnable = true);

    /// if true, then write the message to standard error output as well
    bool isPipedError() const;

    void setPipedError(bool pEnable = true);

    /// Log the process id with each message
    bool isLogPID() const;

    void setLogPID(bool pEnable = true);

    /// Return the flags of the settings
    int flags() const { return m_Options; }

    /// Different syslog needs different facility level.
    void setFacility(int pLabel) { m_Facility = pLabel; }

    int facility() const { return m_Facility; }

  private:
    std::string m_Indent;
    int m_Options;
    int m_Facility;
  };

public:
  SysLog();

  explicit SysLog(const Settings& pSettings);

  ~SysLog();

  void setup(const Settings& pSettings);

  void start();

  void stop();

  bool isStarted() const { return m_bOpened; }

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
  Settings m_Settings;
  bool m_bOpened;
};

} // namespace of diagnostic
} // namespace of onnc

#endif
