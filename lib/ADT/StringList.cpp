//===- StringList.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/ADT/StringList.h>
#include <onnc/Support/OStrStream.h>

namespace onnc {

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
StringRef::size_type SplitString(StringList& pResult,
                                 StringRef pString,
                                 StringRef pSeparator)
{
  StringRef::size_type idx = pString.find(pSeparator);
  if (idx == StringRef::npos) {
    pResult.push_back(pString);
    return idx;
  }
  pResult.push_back(pString.slice(0, idx));
  return SplitString(pResult, pString.slice(idx+1, StringRef::npos), pSeparator);
}

std::string JoinString(const StringList& pList, StringRef pSeparator)
{
  std::string result;
  OStrStream oss(result);
  StringList::const_iterator str, strEnd = pList.end();
  for (str = pList.begin(); str != strEnd; ++str) {
    if (!result.empty())
      oss << pSeparator;
    oss << *str;
  }
  return result;
}

} // namespace of onnc
