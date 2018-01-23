//===- EditMatch.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_EDIT_MATCH_H
#define ONNC_SUPPORT_EDIT_MATCH_H
#include <onnc/ADT/StringRef.h>
#include <onnc/Support/CArgu.h>
#include <onnc/Support/Path.h>

namespace onnc {

inline bool EditMatch(StringRef pX, StringRef pY)
{
  if (!pX.isValid() || !pY.isValid())
    return false;

  StringRef::size_type offset = 0;
  StringRef::iterator xcur, ycur;

  xcur = pX.begin();
  ycur = pY.begin();

  // while not meet any terminal
  while (pX.end() != (xcur + offset) && pY.end() != (ycur + offset)) {
    if (*(xcur + offset) != *(ycur + offset))
      break;
    ++offset;
  }

  // identical strings
  if (0 != offset && (pX.end() == (xcur + offset)) && (pY.end() == (ycur + offset)))
    return true;

  // match only '-'
  if (0 < offset) {
    StringRef common(xcur, offset);
    if (StringRef::npos == common.find_first_not_of('-'))
      return false;
  }

  Path p1(xcur + offset);
  Path p2(ycur + offset);
  return (p1.basename() == p2.basename());
}

inline bool EditMatch(const CArgu& pX, const CArgu& pY)
{
  if (pX.size() != pY.size())
    return false;
  CArgu::const_iterator xargu, yargu, xend = pX.end();
  for (xargu = pX.begin(), yargu = pY.begin(); xargu != xend; ++xargu, ++yargu) {
    if (!EditMatch(*xargu, *yargu))
      return false;
  }
  return true;
}

} // namespace of onnc

#endif
