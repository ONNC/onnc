//===- Glob.inc -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <glob.h>

namespace onnc {

class GlobPriv
{
public:
  GlobPriv();

  ~GlobPriv();

  Glob::Result match(const std::string& pPattern, Glob::PathList& pPathnames);

private:
  ::glob_t m_Glob;
};

//===----------------------------------------------------------------------===//
// Implementation of GlobPriv
//===----------------------------------------------------------------------===//
GlobPriv::GlobPriv()
{
}

GlobPriv::~GlobPriv()
{
  ::globfree(&m_Glob);
}

Glob::Result
GlobPriv::match(const std::string& pPattern, Glob::PathList& pPathnames)
{
  m_Glob.gl_offs = pPathnames.size();
  int result = ::glob(pPattern.c_str(), GLOB_DOOFFS | GLOB_NOSORT, NULL, &m_Glob);
  switch (result) {
    case GLOB_NOSPACE:
      return Glob::kNoSpace;
    case GLOB_ABORTED:
      return Glob::kAborted;
    case GLOB_NOMATCH:
      return Glob::kNoMatch;
  }

  for (int i = 0; i < m_Glob.gl_pathc; ++i)
    pPathnames.push_back(m_Glob.gl_pathv[i + m_Glob.gl_offs]);
  return Glob::kSuccess;
}

} // namespace of onnc
