//===- MsgHandler.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Diagnostic/MsgHandler.h>
#include <onnc/Diagnostic/Engine.h>
#include <cassert>

using namespace onnc;
using namespace onnc::diagnostic;

//===----------------------------------------------------------------------===//
// MsgHandler
//===----------------------------------------------------------------------===//
MsgHandler::MsgHandler(Engine& pEngine)
  : m_Engine(pEngine), m_NumOfArgs(0) {
}

MsgHandler::~MsgHandler()
{
  emit();
}

bool MsgHandler::emit()
{
  flushCounts();
  return m_Engine.emit();
}

/// add an argument as a std::string.
void MsgHandler::addString(const std::string& pStr) const
{
  assert(m_NumOfArgs <= State::MaxArguments &&
         "Too many arguments in a diagnostic");
  m_Engine.state().ArgumentKinds[m_NumOfArgs] = ak_std_string;
  m_Engine.state().ArgumentStrs[m_NumOfArgs++] = pStr.data();
}

/// add an argument with tagged value.
void MsgHandler::addTaggedValue(intptr_t pValue, ArgumentKind pKind) const
{
  assert(m_NumOfArgs <= State::MaxArguments &&
         "Too many arguments in a diagnostic");
  m_Engine.state().ArgumentKinds[m_NumOfArgs] = pKind;
  m_Engine.state().ArgumentVals[m_NumOfArgs++] = pValue;
}

void MsgHandler::flushCounts()
{
  m_Engine.state().NumOfArgs = m_NumOfArgs;
}
