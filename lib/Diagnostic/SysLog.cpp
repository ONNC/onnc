//===- SysLog.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Diagnostic/SysLog.h>
#include <onnc/ADT/Rope.h>
#include <assert.h>
#include <syslog.h>

using namespace onnc;
using namespace onnc::diagnostic;

//===----------------------------------------------------------------------===//
// SysLog::Settings
//===----------------------------------------------------------------------===//
SysLog::Settings::Settings()
  : m_Indent(), m_Options(0), m_Facility(0) {
}

SysLog::Settings::~Settings()
{
  m_Indent.clear();
  m_Options = 0;
  m_Facility = 0;
}

bool SysLog::Settings::isLogConsole() const
{
  return (0x0 != (flags() & LOG_CONS));
}

void SysLog::Settings::setLogConsole(bool pEnable)
{
  int mask = LOG_CONS;
  m_Options = (pEnable)? (m_Options | mask) : (m_Options & ~mask);
}

bool SysLog::Settings::isNotDelay() const
{
  return (0x0 != (flags() & LOG_NDELAY));
}

void SysLog::Settings::setNotDelay(bool pEnable)
{
  int mask = LOG_NDELAY;
  m_Options = (pEnable)? (m_Options | mask) : (m_Options & ~mask);
}

bool SysLog::Settings::isPipedError() const
{
  return (0x0 != (flags() & LOG_PERROR));
}

void SysLog::Settings::setPipedError(bool pEnable)
{
  int mask = LOG_PERROR;
  m_Options = (pEnable)? (m_Options | mask) : (m_Options & ~mask);
}

bool SysLog::Settings::isLogPID() const
{
  return (0x0 != (flags() & LOG_PID));
}

void SysLog::Settings::setLogPID(bool pEnable)
{
  int mask = LOG_PID;
  m_Options = (pEnable)? (m_Options | mask) : (m_Options & ~mask);
}

//===----------------------------------------------------------------------===//
// SysLogger
//===----------------------------------------------------------------------===//
SysLog::SysLog()
  : Logger(100, 100), m_Settings(), m_bOpened(false) {
}

SysLog::SysLog(const Settings& pSettings)
  : Logger(100, 100), m_Settings(pSettings), m_bOpened(false) {
}

SysLog::~SysLog()
{
  if (isStarted())
    stop();
}

void SysLog::setup(const SysLog::Settings& pSettings)
{
  m_Settings = pSettings;
}

void SysLog::start()
{
  if (!isStarted()) {
    ::openlog(m_Settings.indent().c_str(), m_Settings.flags(), m_Settings.facility());
    m_bOpened = true;
  }
}

void SysLog::stop()
{
  if (isStarted()) {
    ::closelog();
    m_bOpened = false;
  }
}

void SysLog::unreachable(const std::string& pMessage)
{
  assert(isStarted() && "SysLog not started");
  std::string mesg = (Rope(pMessage) +
                      Rope("\n\nYou meet a bug of ONNC. Please report to:\n") +
                      Rope("  bugs@onnc.ai")).str();
  ::syslog(LOG_EMERG, "Unreachable: %s\n", mesg.c_str());
}

void SysLog::fatal(const std::string& pMessage)
{
  assert(isStarted() && "SysLog not started");
  ::syslog(LOG_ALERT, "Fatal: %s\n", pMessage.c_str());
}

void SysLog::error(const std::string& pMessage)
{
  assert(isStarted() && "SysLog not started");
  ::syslog(LOG_ERR, "Error: %s\n", pMessage.c_str());
}

void SysLog::warning(const std::string& pMessage)
{
  assert(isStarted() && "SysLog not started");
  ::syslog(LOG_WARNING, "Warning: %s\n", pMessage.c_str());
}

void SysLog::debug(const std::string& pMessage)
{
  assert(isStarted() && "SysLog not started");
  ::syslog(LOG_DEBUG, "Debug: %s\n", pMessage.c_str());
}

void SysLog::note(const std::string& pMessage)
{
  assert(isStarted() && "SysLog not started");
  ::syslog(LOG_INFO, "Note: %s\n", pMessage.c_str());
}

void SysLog::ignore(const std::string& pMessage)
{
  assert(isStarted() && "SysLog not started");
  ::syslog(LOG_NOTICE, "Ignore: %s\n", pMessage.c_str());
}

void SysLog::printTooManyErrors()
{
  assert(isStarted() && "SysLog not started");
  ::syslog(LOG_CRIT, "\n\ntoo many error message (>%d)...\n", getMaxErrors());
}

void SysLog::printTooManyWarnings()
{
  assert(isStarted() && "SysLog not started");
  ::syslog(LOG_CRIT, "\n\ntoo many warning message (>%d)...\n", getMaxWarnings());
}
