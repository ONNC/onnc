//===- Environ.cpp --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/Environ.h>
#include <onnc/Support/CEnv.h>
#include <unistd.h>

using namespace onnc;

extern char** environ;

//===----------------------------------------------------------------------===//
// Environ
//===----------------------------------------------------------------------===//
unsigned int Environ::size()
{
  if (nullptr == environ)
    return 0;

  unsigned int rst = 0;
  while (nullptr != environ[rst])
    ++rst;

  return rst;
}

Environ::const_reference Environ::at(size_type pIdx)
{
  return const_reference(environ[pIdx]);
}

bool Environ::set(StringRef pName, StringRef pValue, bool pOverwrite)
{
  if (!pName.isValid() || pName.empty())
    return false;

  return (0 == setenv(pName.c_str(), pValue.c_str(), pOverwrite));
}

Environ::const_reference Environ::get(StringRef pName)
{
  if (!pName.isValid() || pName.empty())
    return const_reference();
  return const_reference(getenv(pName.c_str()));
}

bool Environ::has(StringRef pName)
{
  return (NULL != getenv(pName.c_str()));
}

bool Environ::unset(StringRef pName)
{
  if (!pName.isValid() || pName.empty())
    return false;
  return (0 == unsetenv(pName.c_str()));
}

CEnv::const_iterator Environ::insert(const CEnv& pCEnv, bool pOverwrite)
{
  CEnv::const_iterator it, itEnd = pCEnv.end();
  for (it = pCEnv.begin(); it != itEnd; ++it) {
    if (!set(it->key(), it->value(), pOverwrite))
      return it;
  }
  return it;
}

char** Environ::raw()
{
  return environ;
}

//===----------------------------------------------------------------------===//
// Environ::iterator
//===----------------------------------------------------------------------===//
Environ::iterator& Environ::iterator::operator=(const Environ::iterator& pCopy)
{
  m_Idx = pCopy.m_Idx;
  return *this;
}

Environ::iterator& Environ::iterator::operator++()
{
  ++m_Idx;
  if (!Environ::at(m_Idx).isValid())
    m_Idx = -1;
  return *this;
}

Environ::iterator& Environ::iterator::operator--()
{
  --m_Idx;
  if (!Environ::at(m_Idx).isValid())
    m_Idx = -1;
  return *this;
}

Environ::iterator Environ::iterator::operator++(int)
{
  iterator tmp(*this);
  ++m_Idx;
  if (!Environ::at(m_Idx).isValid())
    m_Idx = -1;
  return tmp;
}

Environ::iterator Environ::iterator::operator--(int)
{
  iterator tmp(*this);
  --m_Idx;
  if (!Environ::at(m_Idx).isValid())
    m_Idx = -1;
  return tmp;
}

Environ::const_reference Environ::iterator::key() const
{
  std::pair<StringRef, StringRef> p = Environ::at(m_Idx).split('=');
  return p.first;
}

Environ::const_reference Environ::iterator::value() const
{
  std::pair<StringRef, StringRef> p = Environ::at(m_Idx).split('=');
  return p.second;
}
