//===- DiagnosticEngine.cpp -----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Diagnostic/Engine.h>
#include <onnc/Diagnostic/Diagnostic.h>
#include <onnc/Diagnostic/Policy.h>
#include <onnc/Diagnostic/StreamLog.h>
#include <onnc/Diagnostic/MsgHandler.h>
#include <onnc/Support/IOStream.h>
#include <cassert>

using namespace onnc;
using namespace onnc::diagnostic;

//===----------------------------------------------------------------------===//
// Engine
//===----------------------------------------------------------------------===//
Engine::Engine()
  : m_pLogger(new StreamLog(outs())) {
  m_pLogger->start();
}

Engine::~Engine()
{
  m_pLogger->stop();
  delete m_pLogger;
}

void Engine::delegate(Logger* pLogger)
{
  if (nullptr != pLogger) {
    m_pLogger->stop();
    delete m_pLogger;
    m_pLogger = pLogger;
    m_pLogger->start();
  }
}

bool Engine::hasError() const
{
  return (m_pLogger->getNumErrors() > 0);
}

diagnostic::MsgHandler
diagnostic::Engine::report(unsigned int pID, Severity pSeverity)
{
  state().ID = pID;
  state().CurrentSeverity = pSeverity;
  state().Format = m_InfoMap.description(pID);

  // The desturctor of MsgHandler calls back Engine::emit()
  MsgHandler result(*this);
  return result;
}

diagnostic::MsgHandler
diagnostic::Engine::report(StringRef pMesg, Severity pSeverity)
{
  state().ID = generic_note;
  state().CurrentSeverity = pSeverity;
  state().Format = pMesg;

  // The desturctor of MsgHandler calls back Engine::emit()
  MsgHandler result(*this);
  return result;
}

//===----------------------------------------------------------------------===//
// Engine::State
//===----------------------------------------------------------------------===//
diagnostic::State::State()
  : Format(), NumOfArgs(0), ID(0), CurrentSeverity(None) {
}

void diagnostic::State::reset()
{
  Format          = StringRef();
  NumOfArgs       = 0;
  ID              = 0;
  CurrentSeverity = None;
}
