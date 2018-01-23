//===- CArgu.cpp ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/CArgu.h>
#include <onnc/Support/Expansion.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/Support/OStrStream.h>
#include <sstream>
#include <cstring>
#include <cstdlib>

#include <onnc/Support/IOStream.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// CArgu
//===----------------------------------------------------------------------===//
CArgu::CArgu()
  : m_Argc(0), m_Argv(nullptr), m_Capacity(0) {
}

CArgu::CArgu(int pArgc, char* pArgv[])
  : m_Argc(0), m_Argv(nullptr), m_Capacity(0) {
  for (int i = 0; i < pArgc; ++i) {
    if (nullptr != pArgv[i])
      doAppend(::strdup(pArgv[i]));
    else
      doAppend(nullptr);
  }
}

CArgu::CArgu(int pArgc, const char* pArgv[])
  : m_Argc(0), m_Argv(nullptr), m_Capacity(0) {
  for (int i = 0; i < pArgc; ++i) {
    if (nullptr != pArgv[i])
      doAppend(::strdup(pArgv[i]));
    else
      doAppend(nullptr);
  }
}

CArgu::CArgu(StringRef pCommand)
  : m_Argc(0), m_Argv(nullptr), m_Capacity(0) {
  append(pCommand);
}

CArgu::CArgu(const CArgu& pOther)
  : m_Argc(0), m_Argv(nullptr), m_Capacity(0) {
  for (int i = 0; i < pOther.size(); ++i) {
    if (nullptr != pOther[i])
      doAppend(::strdup(pOther[i]));
    else
      doAppend(nullptr);
  }
}

CArgu::CArgu(CArgu&& pOther)
  : m_Argc(0), m_Argv(nullptr), m_Capacity(0) {
  for (int i = 0; i < pOther.size(); ++i) {
    doAppend(pOther[i]);
  }
  pOther.clear();
}

CArgu::~CArgu()
{
  clear();
}

CArgu& CArgu::operator=(const CArgu& pOther)
{
  clear();

  for (int i = 0; i < pOther.size(); ++i) {
    if (nullptr != pOther[i])
      doAppend(::strdup(pOther[i]));
    else
      doAppend(nullptr);
  }
  return *this;
}

CArgu& CArgu::operator=(CArgu&& pOther)
{
  clear();

  for (int i = 0; i < pOther.size(); ++i) {
    doAppend(pOther[i]);
  }

  // do not clear pOther[i]
  ::free(pOther.m_Argv);
  pOther.m_Argc = 0;
  pOther.m_Capacity = 0;
  pOther.m_Argv = nullptr;
  return *this;
}

CArgu& CArgu::assign(StringRef pCommandLine)
{
  clear();
  return append(pCommandLine);
}

CArgu& CArgu::assign(int pArgc, char* pArgv[])
{
  clear();
  return append(pArgc, pArgv);
}

CArgu& CArgu::assign(int pArgc, const char* pArgv[])
{
  clear();
  return append(pArgc, pArgv);
}

CArgu& CArgu::assign(const CArgu& pArgu)
{
  clear();
  return operator=(pArgu);
}

CArgu& CArgu::append(StringRef pCommand)
{
  if (!pCommand.isValid())
    return *this;

  StringRef::size_type start = 0;
  StringRef::size_type current = 0;
  while (current < pCommand.size()) {
    switch (pCommand[current]) {
      case ' ':
      case '\t': {
        StringRef token;
        if ('"' == pCommand[start] || '\'' == pCommand[start])
          token = pCommand.substr(start + 1, current - start - 2);
        else
          token = pCommand.substr(start, current - start);

        if (0 == token.size())
          doAppend(nullptr);
        else
          doAppend(::strndup(token.data(), token.length()));

        do {
          ++current;
        } while (' ' == pCommand[current] || '\t' == pCommand[current]);
        start = current;
        break;
      }
      case '"' :
      case '\'' : {
        char delimiter = pCommand[current];
        ++current;
        while (current < pCommand.size()) {
          if (delimiter == pCommand[current]) {
            ++current;
            break;
          }
          ++current;
        } // end of while
        break;
      } // quotation
      default:
        ++current;
    } // end of switch
  } // end of while

  StringRef::size_type end = pCommand.size();
  if ('"' == pCommand[start] || '\'' == pCommand[start]) {
    start += 1;
    if ('"' == pCommand[end - 1] || '\'' == pCommand[end - 1])
      end -= 1;
  }

  StringRef token = pCommand.substr(start, end - start);
  if (0 == token.length())
    doAppend(nullptr);
  else
    doAppend(::strndup(token.data(), token.length()));
  return *this;
}

CArgu& CArgu::append(int pArgc, char* pArgv[])
{
  for (int i = 0; i < pArgc; ++i) {
    if (nullptr != pArgv[i])
      doAppend(::strdup(pArgv[i]));
    else
      doAppend(nullptr);
  }

  return *this;
}

CArgu& CArgu::append(int pArgc, const char* pArgv[])
{
  for (int i = 0; i < pArgc; ++i) {
    if (nullptr != pArgv[i])
      doAppend(::strdup(pArgv[i]));
    else
      doAppend(nullptr);
  }

  return *this;
}

CArgu& CArgu::append(const CArgu& pOther)
{
  CArgu::const_iterator argu, aEnd = pOther.end();
  for (argu = pOther.begin(); argu != aEnd; ++argu) {
    if (nullptr != *argu)
      doAppend(::strdup(*argu));
    else
      doAppend(nullptr);
  }

  return *this;
}

CArgu& CArgu::doAppend(char* pString)
{
  // argument arrary ends with nullptr.
  if ((m_Argc + 1) >= m_Capacity) {
    m_Capacity = 2*m_Capacity + 2;
    m_Argv = (char**)realloc(m_Argv, m_Capacity*sizeof(char*));
  }
  m_Argv[m_Argc] = pString;
  ++m_Argc;
  m_Argv[m_Argc] = nullptr;
  return *this;
}

void CArgu::clear()
{
  if (nullptr != m_Argv) {
    for (int i = 0; i < size(); ++i) {
      if (nullptr != at(i))
        ::free(at(i));
    }
    ::free(m_Argv);
  }

  m_Argv = nullptr;
  m_Argc = 0;
  m_Capacity = 0;
}

// read operator
const char* CArgu::at(unsigned int pIdx) const
{
  if (m_Argc <= pIdx)
    fatal(fatal_out_of_range) << pIdx << m_Argc;

  return m_Argv[pIdx];
}

/// write operator
CArgu::reference CArgu::at(unsigned int pIdx)
{
  if (m_Argc <= pIdx)
    fatal(fatal_out_of_range) << pIdx << m_Argc;

  return reference(m_Argv[pIdx]);
}

CArgu::reference CArgu::operator[](unsigned int pIdx)
{
  return reference(m_Argv[pIdx]);
}

const char* CArgu::operator[](unsigned int pIdx) const
{
  return m_Argv[pIdx];
}

CArgu& CArgu::push_back(const char* pString)
{
  if (nullptr == pString)
    return doAppend(nullptr);
  return doAppend(::strdup(pString));
}

void CArgu::print(std::ostream& pOS) const
{
  int i = 0;
  while (i < this->size()) {
    if (nullptr != this->at(i)) {
      StringRef::size_type space = StringRef(this->at(i)).find(' ');
      StringRef::size_type tab   = StringRef(this->at(i)).find('\t');
      if (StringRef::npos != space || StringRef::npos != tab)
        pOS << '\"' << this->at(i) << '\"';
      else
        pOS << this->at(i);
    }

    if (++i != this->size())
      pOS << " ";
  }
}

void CArgu::print(std::string& pStr) const
{
  OStrStream oss(pStr);
  print(oss);
}

std::string CArgu::getCommandLine() const
{
  std::ostringstream oss;
  print(oss);
  return oss.str();
}

void CArgu::free(int& pArgc, char**& pArgv)
{
  for (int i = 0; i < pArgc; ++i) {
    if (nullptr != pArgv[i])
      ::free(pArgv[i]);
  }
  ::free(pArgv);

  pArgc = 0;
  pArgv = nullptr;
}

void CArgu::give(int& pArgc, char**& pArgv)
{
  pArgc = m_Argc;
  pArgv = m_Argv;

  m_Argc = m_Capacity = 0;
  m_Argv = nullptr;
}

//===----------------------------------------------------------------------===//
// Non-member function
//===----------------------------------------------------------------------===//
bool onnc::expand(CArgu& pResult, const CArgu& pOrig)
{
  CArgu::const_iterator argu, aEnd = pOrig.end();
  for (argu = pOrig.begin(); argu != aEnd; ++argu) {
    Expansion exp(*argu);
    if (!exp.isGood())
      return false;
    StringList words(exp.words());
    StringList::const_iterator word, wEnd = words.end();
    for (word = words.begin(); word != wEnd; ++word) {
      pResult.push_back(word->c_str());
    }
  }
  return true;
}
