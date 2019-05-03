//===- Path.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/Path.h>
#include <onnc/Config/Config.h>
#include <onnc/Support/FileSystem.h>
#include <cstring>
#include <cassert>
#include <string>

// Include the truly platform-specific parts.
#if defined(ONNC_ON_UNIX)
#include "Unix/Path.inc"
#endif
#if defined(ONNC_ON_WIN32)
#include "Windows/Path.inc"
#endif
#if defined(ONNC_ON_DRAGON)
#include "Dragon/Path.inc"
#endif

using namespace onnc;

const char Path::colon = ':';
const char Path::dot   = '.';

//===--------------------------------------------------------------------===//
// Path
//===--------------------------------------------------------------------===//
Path::Path()
  : m_PathName() {
}

Path::Path(const Path::ValueType* pStr)
  : m_PathName() {
  if (NULL != pStr)
    m_PathName = pStr;
}

Path::Path(const Path::StringType &s )
  : m_PathName(s) {
}

Path::Path(const Path& pCopy)
 : m_PathName(pCopy.m_PathName) {
}

Path::Path(size_t pN, char pChar)
  : m_PathName(pN, pChar) {
}

Path::~Path()
{
}

bool Path::isFromRoot() const
{
  if (m_PathName.empty())
    return false;
  return (separator == m_PathName[0]);
}

bool Path::isFromPWD() const
{
  if (m_PathName.empty())
    return false;

  /* . */
  if (1 == m_PathName.size() && '.' == m_PathName[0])
    return true;

  /* .. or ./ */
  if (2 == m_PathName.size() && '.' == m_PathName[0]) {
    if ('.' == m_PathName[1] || separator == m_PathName[1])
      return true;
  }

  // ../
  return ('.' == m_PathName[0] && '.' == m_PathName[1] &&
          separator == m_PathName[2]);
}

bool Path::equals(const char* pString) const
{
  return (0 == ::strcmp(pString, m_PathName.c_str()));
}

int Path::compare(const char* pString) const
{
  return ::strcmp(pString, m_PathName.c_str());
}

Path& Path::assign(const Path& pCopy)
{
  m_PathName.assign(pCopy.native());
  return *this;
}

Path& Path::assign(const Path::ValueType* s, unsigned int length)
{
  if (0 == s || 0 == length)
    assert(0 && "assign a null or empty string to Path");
  m_PathName.assign(s, length);
  return *this;
}

//a,/b a/,b a/,b/ a,b is a/b
Path& Path::append(const Path& pPath)
{
  //first path is a/,second path is /b
  if (m_PathName[m_PathName.length()-1] == separator &&
      pPath.native()[0] == separator) {
    unsigned int old_size = m_PathName.size()-1;
    unsigned int new_size = old_size + pPath.native().size();

    m_PathName.resize(new_size);
    strcpy(const_cast<ValueType*>(m_PathName.data()+old_size), pPath.native().data());
  }
  //first path is a,second path is b
  else if (this->native()[this->native().size()-1] != separator &&
           pPath.native()[0] != separator) {
    m_PathName.append(separator_str);
    m_PathName.append(pPath.native());
  }
  // a/,b or a,/b just append
  else {
    m_PathName.append(pPath.native());
  }
  return *this;
}

bool Path::empty() const
{
  return m_PathName.empty();
}

Path::StringType Path::generic_string() const
{
  StringType result(m_PathName);
  // On Windows platform, the generic format just replace `\\` with `/`.
  size_t cur_pos = 0;
  StringType from_token("\\");
  StringType to_token("/");
  while((cur_pos = result.find("\\", cur_pos)) != std::string::npos) {
    result.replace(cur_pos, from_token.length(), to_token);
    cur_pos += to_token.length();
  }
  // On POSIX-like systems, the generic format is the same as the native format
  return result;
}

Path::StringType::size_type Path::AppendSeparatorAsNeeded()
{
#if defined(ONNC_ON_WIN32)
  // On Windows platform, path can not append separator.
  return 0;
#endif

  StringType::value_type last_char = m_PathName[m_PathName.size() - 1];
  if (!m_PathName.empty() &&
      !sys::is_separator(last_char)) {
    StringType::size_type tmp(m_PathName.size());
    m_PathName += separator_str;
    return tmp;
  }
  return 0;
}

void Path::EraseRedundantSeparator(Path::StringType::size_type pSepPos)
{
  size_t begin=pSepPos;
  // skip '/' or '\\'
  while (separator == m_PathName[pSepPos]) {
#if defined(ONNC_ON_WIN32)
    pSepPos += 2;
#else
    ++pSepPos;
#endif
  }

  if (begin != pSepPos)
    m_PathName.erase(begin+1,pSepPos-begin-1);
}

Path Path::parent() const
{
  size_t end_pos = m_PathName.find_last_of(separator);
  if (end_pos != StringType::npos)
    return Path(m_PathName.substr(0, end_pos));

  Path workingDirectory;
  getwd(workingDirectory);
  return workingDirectory;
}

Path Path::above() const
{
  size_t start_pos = StringType::npos;
  if (separator == m_PathName.back())
    start_pos = m_PathName.size() - 2; //< also remove '/'
  size_t end_pos = m_PathName.find_last_of(separator, start_pos);
  if (end_pos != StringType::npos)
    return Path(m_PathName.substr(0, end_pos));
  return Path();
}

Path Path::filename() const
{
  size_t pos = m_PathName.rfind(separator);
  if (pos != StringType::npos) {
    ++pos;
    return Path(m_PathName.substr(pos));
  }
  return Path(*this);
}

Path Path::basename() const
{
  size_t last = m_PathName.rfind(separator);
  if (last == (m_PathName.size() - 1)) {
    // directory name
    size_t start = m_PathName.rfind(separator, last-1);
    if (StringType::npos == start)
      return Path(m_PathName.substr(0, last-start-1));
    return Path(m_PathName.substr(start+1, last-start-1));
  }
  else if (last != StringType::npos) {
    // file name
    return Path(m_PathName.substr(last + 1));
  }
  return Path(*this);
}

Path Path::stem() const
{
  size_t begin_pos = m_PathName.find_last_of(separator)+1;
  size_t end_pos   = m_PathName.find_last_of(dot);
  Path result_path(m_PathName.substr(begin_pos, end_pos - begin_pos));
  return result_path;
}

Path Path::extension() const
{
  size_t begin_pos = m_PathName.find_last_of('.');
  Path result_path(m_PathName.substr(begin_pos+1));
  return result_path;
}

bool Path::inside(const Path& pOther) const
{
  return (std::string::npos != m_PathName.find(pOther.native()));
}

//===--------------------------------------------------------------------===//
// non-member functions
//===--------------------------------------------------------------------===//
bool onnc::operator==(const Path& pLHS,const Path& pRHS)
{
  return (pLHS.generic_string()==pRHS.generic_string());
}

bool onnc::operator!=(const Path& pLHS,const Path& pRHS)
{
  return !(pLHS==pRHS);
}

Path onnc::operator+(const Path& pLHS, const Path& pRHS)
{
  Path result = pLHS;
  result.append(pRHS);
  return result;
}

template<> onnc::TypeTag onnc::type_tag<Path>()
{
  static bool instance;
  return &instance;
}
