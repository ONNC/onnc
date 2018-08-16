//===- StringList.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ADT_STRING_LIST_H
#define ONNC_ADT_STRING_LIST_H
#include <onnc/ADT/StringRef.h>
#include <vector>

namespace onnc {

typedef std::vector<StringRef> StringList;

/// Split the string into a list of substrings by
/// all occurrence of the separator.
///
/// @param[out] pResult The resulting list of substrings.
/// @param[in]  pString The input string to be splitted.
/// @return     The position of the last separator.
StringRef::size_type SplitString(StringList& pResult,
                                 StringRef pString,
                                 StringRef pSeparator);

/// Join the string list into a string by the given separator.
///
/// @param[in]  pList      The string list to be joined.
/// @param[in]  pSeparator Separator used to join the string list.
/// @return                The resulting string.
std::string JoinString(const StringList& pList, StringRef pSeparator);

} // namespace of onnc

#endif
