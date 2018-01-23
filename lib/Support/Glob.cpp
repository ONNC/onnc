//===- Glob.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/Glob.h>
#include <onnc/Config/Config.h>
// Include the truly platform-specific parts. 
#if defined(ONNC_ON_UNIX)
#include "Unix/Glob.inc"
#endif
#if defined(ONNC_ON_WIN32)
#include "Windows/Glob.inc"
#endif
#if defined(ONNC_ON_DRAGON)
#include "Dragon/Glob.inc"
#endif

using namespace onnc;

//===----------------------------------------------------------------------===//
// Glob
//===----------------------------------------------------------------------===//
Glob::Glob(const Path& pPattern)
  : m_Pattern(pPattern), m_pPriv(new GlobPriv()) {
}

Glob::~Glob()
{
  delete m_pPriv;
}

/// Generate pathnames matching the pattern.
Glob::Result Glob::match(PathList& pPathnames)
{
  return m_pPriv->match(m_Pattern.native(), pPathnames);
}
