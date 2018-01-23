//===- OFStreamLog.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Diagnostic/OFStreamLog.h>
#include <onnc/Diagnostic/StreamLog.h>

using namespace onnc;
using namespace onnc::diagnostic;

//===----------------------------------------------------------------------===//
// OFStreamLog
//===----------------------------------------------------------------------===//
OFStreamLog::OFStreamLog()
  : StreamLog(m_OS), m_OS(), m_LogFile() {
}

OFStreamLog::OFStreamLog(const Path& pLogFile)
  : StreamLog(m_OS), m_OS(pLogFile), m_LogFile(pLogFile) {
}

OFStreamLog::~OFStreamLog()
{
  if (m_OS.is_open())
    m_OS.close();
}
