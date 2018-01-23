//===- TextDiagnosticPrinter.cpp ------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Diagnostic/Logger.h>
#include <onnc/Diagnostic/Diagnostic.h>
#include <onnc/Diagnostic/GeneralOptions.h>
#include <onnc/Support/IOStream.h>
#include <cstdlib>

using namespace onnc;
using namespace onnc::diagnostic;

//===----------------------------------------------------------------------===//
// Logger
//===----------------------------------------------------------------------===//
Logger::Logger(unsigned int pMaxWarning, unsigned int pMaxErrors)
  : m_MaxWarnings(pMaxWarning), m_NumWarnings(0),
    m_MaxErrors(pMaxErrors), m_NumErrors(0) {
}

bool Logger::handle(const Diagnostic& pDiag)
{
  if (Warning == pDiag.severity())
    ++m_NumWarnings;

  if (Error >= pDiag.severity())
    ++m_NumErrors;

  std::string out_str;
  pDiag.format(out_str);
  switch (pDiag.severity()) {
    case Unreachable: {
      unreachable(out_str);
      exit(EXIT_FAILURE);
      break;
    }
    case Fatal: {
      fatal(out_str);
      exit(EXIT_FAILURE);
      break;
    }
    case Error: {
      if (this->getNumErrors() > this->getMaxErrors()) {
        printTooManyErrors();
        exit(EXIT_FAILURE);
      }
      error(out_str);
      break;
    }
    case Warning: {
      if (this->getNumWarnings() > this->getMaxWarnings()) {
        printTooManyWarnings();
        exit(EXIT_FAILURE);
      }
      warning(out_str);
      break;
    }
    case Debug: {
      debug(out_str);
      break;
    }
    case Note: {
      note(out_str);
      break;
    }
    case Ignore: {
      ignore(out_str);
      break;
    }
    default:
      return false;
  } // end of switch
  return true;
}
