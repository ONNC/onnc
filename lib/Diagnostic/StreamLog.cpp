//===- StreamLog.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Diagnostic/StreamLog.h>
#include <onnc/Diagnostic/Diagnostic.h>
#include <onnc/Diagnostic/GeneralOptions.h>
#include <onnc/Support/IOStream.h>
#include <cstdlib>

using namespace onnc;
using namespace onnc::diagnostic;

//===----------------------------------------------------------------------===//
// StreamLog
//===----------------------------------------------------------------------===//
StreamLog::StreamLog(OStream& pOStream)
  : Logger(20, 20), m_OStream(pOStream) {
}

void StreamLog::unreachable(const std::string& pMessage)
{
  m_OStream.changeColor(UnreachableColor, true);
  m_OStream << "Unreachable: ";
  m_OStream.resetColor();
  m_OStream << pMessage << "\n\n";
  m_OStream.changeColor(OStream::YELLOW);
  m_OStream << "You meet a bug of ONNC. Please report to:\n"
            << "  bugs@onnc.ai\n";
  m_OStream.resetColor();
}

void StreamLog::fatal(const std::string& pMessage)
{
  m_OStream.changeColor(FatalColor, true);
  m_OStream << "Fatal: ";
  m_OStream.resetColor();
  m_OStream << pMessage << "\n";
}

void StreamLog::error(const std::string& pMessage)
{
  m_OStream.changeColor(ErrorColor, true);
  m_OStream << "Error: ";
  m_OStream.resetColor();
  m_OStream << pMessage << "\n";
}

void StreamLog::warning(const std::string& pMessage)
{
  m_OStream.changeColor(WarningColor, true);
  m_OStream << "Warning: ";
  m_OStream.resetColor();
  m_OStream << pMessage << "\n";
}

void StreamLog::debug(const std::string& pMessage)
{
  m_OStream.changeColor(DebugColor, true);
  m_OStream << "Debug: ";
  m_OStream.resetColor();
  m_OStream << pMessage << "\n";
}

void StreamLog::note(const std::string& pMessage)
{
  m_OStream.changeColor(NoteColor, true);
  m_OStream << "Note: ";
  m_OStream.resetColor();
  m_OStream << pMessage << "\n";
}

void StreamLog::ignore(const std::string& pMessage)
{
  m_OStream.changeColor(IgnoreColor, true);
  m_OStream << "Ignore: ";
  m_OStream.resetColor();
  m_OStream << pMessage << "\n";
}

void StreamLog::printTooManyErrors()
{
  m_OStream << "\n\n";
  m_OStream.changeColor(OStream::YELLOW);
  m_OStream << "too many error messages (>" << this->getMaxErrors() << ")...\n";
  m_OStream.resetColor();
}

void StreamLog::printTooManyWarnings()
{
  m_OStream << "\n\n";
  m_OStream.changeColor(OStream::YELLOW);
  m_OStream << "too many warning messages (>" << this->getMaxWarnings() << ")...\n";
  m_OStream.resetColor();
}
