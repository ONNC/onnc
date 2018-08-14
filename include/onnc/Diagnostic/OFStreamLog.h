//===- OFStreamLog.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_DIAGNOSTIC_OUTFILE_STREAM_LOG_H
#define ONNC_DIAGNOSTIC_OUTFILE_STREAM_LOG_H
#include <onnc/Support/OFStream.h>
#include <onnc/Diagnostic/StreamLog.h>

namespace onnc {
namespace diagnostic {

/** \class OFStreamLog
 *  \brief OFStreamLog display the result of diagnostic in text file
 *
 *  A convenient class to decorate StreamLog
 */
class OFStreamLog : public StreamLog
{
public:
  OFStreamLog();

  OFStreamLog(const Path& pLogFile);

  ~OFStreamLog();

  void setup(const Path& pLogFile) { m_LogFile = pLogFile; }

  void start() { m_OS.open(m_LogFile); }

  void stop() { m_OS.close(); }

  bool isDisplayed() const { m_OS.isDisplayed(); }

private:
  OFStream m_OS;
  Path m_LogFile;
};

} // namespace of diagnostic
} // namespace of onnc

#endif
