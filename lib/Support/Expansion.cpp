//===- Expansion.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/Expansion.h>
#include <onnc/Config/Config.h>
// Include the truly platform-specific parts. 
#if defined(ONNC_ON_UNIX)
#include "Unix/Expansion.inc"
#endif
#if defined(ONNC_ON_WIN32)
#include "Windows/Expansion.inc"
#endif
#if defined(ONNC_ON_DRAGON)
#include "Dragon/Expansion.inc"
#endif

using namespace onnc;

//===----------------------------------------------------------------------===//
// Expansion
//===----------------------------------------------------------------------===//
Expansion::Expansion(StringRef pWord)
  : m_pImpl(new ExpansionImpl(pWord)){
}

Expansion::~Expansion()
{
  delete m_pImpl;
}

unsigned int Expansion::size() const
{
  return m_pImpl->size();
}

StringList Expansion::words() const
{
  StringList words;
  m_pImpl->expand(words);
  return words;
}

Expansion::ErrorType Expansion::error() const
{
  return m_pImpl->error();
}
