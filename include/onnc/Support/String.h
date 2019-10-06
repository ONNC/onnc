//===- String.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_STRING_H_INCLUDED
#define ONNC_SUPPORT_STRING_H_INCLUDED

#include <onnc/Support/IOStream.h>

#include <sstream>
#include <string>
#include <utility>

namespace onnc {

template <typename First, typename Second, typename... Rest>
inline std::string to_string(First&& first, Second&& second, Rest&&... rest)
{
  std::ostringstream stream;
  print(stream, std::forward<First>(first), std::forward<Second>(second), std::forward<Rest>(rest)...);
  return stream.str();
}

} // namespace onnc

#endif
