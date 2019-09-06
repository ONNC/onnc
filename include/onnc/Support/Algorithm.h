//===- Algorithm.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_ALGORITHM_H_INCLUDED
#define ONNC_SUPPORT_ALGORITHM_H_INCLUDED

#include <onnc/Support/Range.h>
#include <onnc/Support/TypeTraits.h>

#include <algorithm>
#include <initializer_list>
#include <type_traits>
#include <utility>

namespace onnc {

template <typename T, typename InputIterator,
          typename = void_t<decltype(std::declval<T>() == *std::declval<InputIterator>())>>
inline constexpr bool equal(std::initializer_list<T> range, InputIterator it)
{
  using std::begin;
  using std::end;

  return std::equal(begin(range), end(range), it);
}

template <typename Range, typename InputIterator,
          typename = void_t<decltype(*std::begin(std::declval<Range>()) == *std::declval<InputIterator>())>>
inline constexpr bool equal(const Range& range, InputIterator it)
{
  using std::begin;
  using std::end;

  return std::equal(begin(range), end(range), it);
}

template <typename Range, typename OutputIterator,
          typename = void_t<decltype(*std::declval<OutputIterator>() = *std::begin(std::declval<Range>()))>>
inline constexpr OutputIterator copy(const Range& range, OutputIterator it)
{
  using std::begin;
  using std::end;

  return std::copy(begin(range), end(range), it);
}

template <typename Left, typename Right>
typename std::common_type<Left, Right>::type min(const Left& lhs, const Right& rhs)
{
  if (lhs < rhs) {
    return lhs;
  } else {
    return rhs;
  }
}

} // namespace onnc

#endif
