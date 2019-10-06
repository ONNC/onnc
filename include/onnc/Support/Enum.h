//===- Enum.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_ENUM_H_INCLUDED
#define ONNC_SUPPORT_ENUM_H_INCLUDED

#include <type_traits>

namespace onnc {

template <typename E, typename = typename std::enable_if<std::is_enum<E>::value>::type>
struct enum_hash
{
  using underlying_type = typename std::underlying_type<E>::type;

  static_assert(std::is_convertible<underlying_type, std::size_t>::value,
                "enum_hash's type argument should convertible to std::size_t");

  constexpr std::size_t operator()(const E& e) const
  {
    return static_cast<std::size_t>(static_cast<underlying_type>(e));
  }
};

template <typename E, typename = typename std::enable_if<std::is_enum<E>::value>::type>
struct enum_less
{
  using underlying_type = typename std::underlying_type<E>::type;

  constexpr bool operator()(const E& lhs, const E& rhs) const
  {
    return static_cast<underlying_type>(lhs) < static_cast<underlying_type>(rhs);
  }
};

} // namespace onnc

#endif
