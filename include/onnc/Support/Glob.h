//===- Glob.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_GLOB_H
#define ONNC_SUPPORT_GLOB_H
#include <onnc/Support/Path.h>
#include <vector>

namespace onnc {

class GlobPriv;

/** \class onnc::Glob
 *  \brief Generate pathnames matching a pattern.
 *  @see Glob(3)
 */
class Glob
{
public:
  enum Result {
    kNoSpace,
    kAborted,
    kNoMatch,
    kSuccess
  };

  typedef std::vector<Path> PathList;

public:
  /// @param[in] pPattern The pattern to be expanded
  Glob(const Path& pPattern);

  ~Glob();

  /// Generate pathnames matching the pattern.
  Result match(PathList& pPathnames);

  /// return the pattern used to match
  Path pattern() const { return m_Pattern.native(); }

private:
  friend class GlobImpl;

private:
  Path m_Pattern;
  GlobPriv* m_pPriv;
};

} // namespace of onnc

#endif
