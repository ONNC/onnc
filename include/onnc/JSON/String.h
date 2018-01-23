//===- String.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_JSON_STRING_H
#define ONNC_JSON_STRING_H
#include <onnc/ADT/StringRef.h>

namespace onnc {
namespace json {

/// trim the string @pString.
/// JSON defines several escaped characters:
///
///   \\\", \\\\, \\/, \\b, \\f, \\n, \\r, \\t, \\u.
///
/// @ref onnc::json::trim function removes the leading backslash and turns
/// the escaped character to ASCII code.
std::string trim(const char* pString);

} // namespace of json
} // namespace of onnc

#endif
