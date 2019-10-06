//===- Match.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_SUPPORT_MATCH_H_INCLUDED
#define ONNC_SUPPORT_MATCH_H_INCLUDED

#include <type_traits>

namespace onnc {

template <typename T, typename = typename std::enable_if<std::is_default_constructible<T>::value &&
                                                         std::is_copy_constructible<T>::value>::type>
class MatchAny
{
public:
  using value_type = T;

public:
  constexpr MatchAny() noexcept(std::is_nothrow_constructible<value_type>::value)
    : initialized_(false)
    , value_()
  {}

  constexpr MatchAny(const value_type& value) noexcept(
    std::is_nothrow_constructible<value_type, const value_type&>::value)
    : initialized_(true)
    , value_(value)
  {}

  MatchAny(const MatchAny&) = default;
  MatchAny(MatchAny&&) = default;

  ~MatchAny() = default;

  MatchAny& operator=(const MatchAny&) = default;
  MatchAny& operator=(MatchAny&&) = default;

  constexpr operator bool() const noexcept { return initialized_; }

  constexpr const value_type& value() const noexcept { return value_; }

private:
  const bool       initialized_;
  const value_type value_;
};

template <typename T>
inline constexpr bool operator==(const T& lhs, const MatchAny<T>& rhs)
{
  return (!rhs || lhs == rhs.value());
}

template <typename T>
inline constexpr bool operator==(const MatchAny<T>& lhs, const T& rhs)
{
  return (!lhs || lhs.value() == rhs);
}

} // namespace onnc

#endif
